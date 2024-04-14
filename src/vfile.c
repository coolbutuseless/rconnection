



#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include <R_ext/Connections.h>

#if ! defined(R_CONNECTIONS_VERSION) || R_CONNECTIONS_VERSION != 1
#error "Unsupported connections API version"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// SEXP   R_new_custom_connection(
//            const char *description,  // the filename related to this particular instance
//            const char *mode,         // read/write/binarymode/textmode
//            const char *class_name,   // 'vfile'
//            Rconnection *ptr          // Rconnection pointer
//        );
//
//  --- C-level entry to create a custom connection object -- */
// The returned value is the R-side instance. To avoid additional call to getConnection()
//  the internal Rconnection pointer will be placed in ptr[0] if ptr is not NULL.
//  It is the responsibility of the caller to customize callbacks in the structure,
//  they are initialized to dummy_ (where available) and null_ (all others) callbacks.
//  Also note that the resulting object has a finalizer, so any clean up (including after
//  errors) is done by garbage collection - the caller may not free anything in the
//  structure explicitly (that includes the con->private pointer!).
 

// struct Rconn {
//     char* class;
//     char* description;
//     int enc; /* the encoding of 'description' */
//     char mode[5];
//     Rboolean text, isopen, incomplete, canread, canwrite, canseek, blocking, 
// 	isGzcon;
//     Rboolean (*open)(struct Rconn *);
//     void (*close)(struct Rconn *); /* routine closing after auto open */
//     void (*destroy)(struct Rconn *); /* when closing connection */
//     int (*vfprintf)(struct Rconn *, const char *, va_list);
//     int (*fgetc)(struct Rconn *);
//     int (*fgetc_internal)(struct Rconn *);
//     double (*seek)(struct Rconn *, double, int, int);
//     void (*truncate)(struct Rconn *);
//     int (*fflush)(struct Rconn *);
//     size_t (*read)(void *, size_t, size_t, struct Rconn *);
//     size_t (*write)(const void *, size_t, size_t, struct Rconn *);
//     int nPushBack, posPushBack; /* number of lines, position on top line */
//     char **PushBack;
//     int save, save2;
//     char encname[101];
//     /* will be iconv_t, which is a pointer. NULL if not in use */
//     void *inconv, *outconv;
//     /* The idea here is that no MBCS char will ever not fit */
//     char iconvbuff[25], oconvbuff[50], *next, init_out[25];
//     short navail, inavail;
//     Rboolean EOF_signalled;
//     Rboolean UTF8out;
//     void *id;
//     void *ex_ptr;
//     void *private;
//     int status; /* for pipes etc */
//     unsigned char *buff;
//     size_t buff_len, buff_stored_len, buff_pos;
// };

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Vfile state. 
//   - This is user/private data stored with the 'Rconn' struct that gets 
//     passed to each callback function
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef struct {
  FILE *fp; 
  int verbosity;
} vfile_state;



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// open()
//  - this may be called explicitly by a user call to open(con, mode)
//  - this is also called implicitly by readBin()/writeBin()/readLines()/writeLines();
//
// Possible Modes
//    - "r" or "rt"    Open for reading in text mode.
//    - "w" or "wt"    Open for writing in text mode.
//    - "a" or "at"    Open for appending in text mode.
//    - "rb"           Open for reading in binary mode.
//    - "wb"           Open for writing in binary mode.
//    - "ab"           Open for appending in binary mode.
//    - "r+", "r+b"    Open for reading and writing.
//    - "w+", "w+b"    Open for reading and writing, truncating file initially.
//    - "a+", "a+b"    Open for reading and appending.
//
// Notes:
//   - Supported modes: r, rt, w, wt, rb, wb
//   - unsupported modes: append, simultaneous read/write
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Rboolean vfile_open(struct Rconn *rconn) {
  
  vfile_state *vstate = (vfile_state *)rconn->private;
  if (vstate->verbosity > 0) Rprintf("vfile_open('%s', mode = '%s')\n", 
      rconn->description, rconn->mode);
  
  if (rconn->isopen) {
    error("vfile(): Connection is already open. Cannot open twice");
  }
  
  if (strchr(rconn->mode, 'a') != NULL) {
    error("vfile() does not support append.");
  } else if (strchr(rconn->mode, '+') != NULL) {
    error("vfile() does not support simultaneous r/w.");
  }
  
  rconn->text   = strchr(rconn->mode, 'b') ? FALSE : TRUE;
  rconn->isopen = TRUE;
  
  if (strchr(rconn->mode, 'w') == NULL) {
    rconn->canread  =  TRUE;
    rconn->canwrite = FALSE;
  } else {
    rconn->canread  = FALSE;
    rconn->canwrite =  TRUE;
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Setup file pointer
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  FILE *fp;
  if (rconn->canread) {
    fp = fopen(rconn->description, "rb");
  } else {
    fp = fopen(rconn->description, "wb");
  }
  if (fp == NULL) {
    error("vfile_(): Couldn't open input file '%s' with mode '%s'", rconn->description, rconn->mode);
  }
  
  vstate->fp = fp;
  
  return TRUE;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Close()
//  - should only change state. No resources should be created/destroyed
//  - all actual destruction should happen in 'destroy()' which is called
//    by the garbage collector.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void vfile_close(struct Rconn *rconn) {
  
  vfile_state *vstate = (vfile_state *)rconn->private;
  if (vstate->verbosity > 0)Rprintf("vfile_close('%s')\n", rconn->description);
  
  rconn->isopen = FALSE;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Close the file
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (vstate->fp) {
    fclose(vstate->fp);
    vstate->fp = NULL;  
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Destroy()
//   - R will destroy the Rbonn struct (?)
//   - R will destroy the Rconnection object (?)
//   - Only really have to take care of 'rconn->private' (?)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void vfile_destroy(struct Rconn *rconn) {
  
  vfile_state *vstate = (vfile_state *)rconn->private;
  if (vstate->verbosity > 0) Rprintf("vfile_destroy()\n");
  
  free(vstate); 
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Not sure what this is for. Just call the standard 'fgetc' callback.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int vfile_fgetc_internal(struct Rconn *rconn) {
  
  vfile_state *vstate = (vfile_state *)rconn->private;
  if (vstate->verbosity > 0) Rprintf("vfile_fgetc_internal()\n");
  
  return rconn->fgetc(rconn);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// seek()
//   - vfile() will not support seeking
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double vfile_seek(struct Rconn *rconn, double x, int y, int z) {
  error("vfile_seek() - not supported");
  return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// truncate
//   - vfile() will not support truncation
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void vfile_truncate(struct Rconn *rconn) {
  error("vfile_truncate() - not supported");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// fflush
//   - vfile will not suport flush()
//   - a flush of buffers to file will only occur during 'close()'
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int vfile_fflush(struct Rconn *rconn) {
  error("vfile_fflush() - not supported\n");
  return 1;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// readBin()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
size_t vfile_read(void *dst, size_t size, size_t nitems, struct Rconn *rconn) {
  
  vfile_state *vstate = (vfile_state *)rconn->private;
  if (vstate->verbosity > 0) Rprintf("vfile_read(size = %zu, nitems = %zu)\n", size, nitems);
  
  size_t nread = fread(dst, size, nitems, vstate->fp);
  
  return nread;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// readLines()
//   - fgetc() called until '\n'. this counts as 1 line.
//   - when EOF reached, return -1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int vfile_fgetc(struct Rconn *rconn) {
  
  vfile_state *vstate = (vfile_state *)rconn->private;
  if (vstate->verbosity > 1) Rprintf("vfile_fgetc()\n");
  
  int c = fgetc(vstate->fp);
  
  return c;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// writeBin()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
size_t vfile_write(const void *src, size_t size, size_t nitems, struct Rconn *rconn) {
  
  vfile_state *vstate = (vfile_state *)rconn->private;
  if (vstate->verbosity > 0) Rprintf("vfile_write(size = %zu, nitems = %zu)\n", size, nitems);
 
  size_t wlen = fwrite(src, 1, size * nitems, vstate->fp);
  
  return wlen;
}


#define BUFSIZE 32768

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// writeLines
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int vfile_vfprintf(struct Rconn *rconn, const char* fmt, va_list ap) {
  
  vfile_state *vstate = (vfile_state *)rconn->private;
  if (vstate->verbosity > 0) Rprintf("vfile_vfprintf(fmt = '%s')\n", fmt);
  
  unsigned char str_buf[BUFSIZE + 1];
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // vsnprintf() return value:
  //   The number of characters written if successful or negative value if an 
  //   error occurred. If the resulting string gets truncated due to buf_size 
  //   limit, function returns the total number of characters (not including the 
  //   terminating null-byte) which would have been written, if the limit 
  //   was not imposed. 
  //
  // So when vsnprintf() overflows the given size, it returns the number of 
  // characters it couldn't write.  Tell it the buffer size is '0' and it
  // will just return how long a buffer would be needed to contain the string!
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int slen = vsnprintf((char *)(str_buf), 0, fmt, ap);
  int wlen = slen;
  if (wlen > BUFSIZE) {
    warning("vfile_vfprintf(): Long string truncated to length = %i\n", BUFSIZE);
    wlen = BUFSIZE;
  }

  
  slen = vsnprintf((char *)(str_buf), BUFSIZE, fmt, ap);
  if (slen < 0) {
    error("vfile_vfprintf(): error in 'vsnprintf()");
  }
  
  fwrite(str_buf, 1, wlen, vstate->fp);
  
  return wlen; // Not sure what is being returned here? bytes written?
}

  
  
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Initialize a vfile() R connection object to return to the user
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP vfile_(SEXP description_, SEXP mode_, SEXP verbosity_) {
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Initialize User State
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  vfile_state *vstate = (vfile_state *)calloc(1, sizeof(vfile_state));
  vstate->verbosity = asInteger(verbosity_);

  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // R will alloc for 'con' within R_new_custom_connection() and then
  // I think it takes responsibility for freeing it later.
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Rconnection con = NULL;
  SEXP rc = PROTECT(R_new_custom_connection(CHAR(STRING_ELT(description_, 0)), "rb", "vfile", &con));
  
  con->isopen     = FALSE; // not open initially.
  con->incomplete =  TRUE; // NFI. Data write hasn't been completed?
  con->text       = FALSE; // binary connection by default
  con->canread    =  TRUE; // read-only for now
  con->canwrite   =  TRUE; // read-only for now
  con->canseek    = FALSE; // not possible in this implementation
  con->blocking   =  TRUE; // blacking IO
  con->isGzcon    = FALSE; // Not a gzcon
  
  // Not sure what this really means, but vfile() is not going to do 
  // any character conversion, so let's pretend any text returned in readLines()
  // is utf8.
  con->UTF8out =  TRUE; 
  con->private = vstate;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Callbacks
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  con->open           = vfile_open;
  con->close          = vfile_close;
  con->destroy        = vfile_destroy;
  con->vfprintf       = vfile_vfprintf;
  con->fgetc          = vfile_fgetc;
  con->fgetc_internal = vfile_fgetc_internal;
  con->seek           = vfile_seek;
  con->truncate       = vfile_truncate;
  con->fflush         = vfile_fflush;
  con->read           = vfile_read;
  con->write          = vfile_write;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Auto open if 'mode' is set to something other than the empty string.
  // An issue is that without the context stuff (not exported from R?), 
  // I don't think I can get the context to auto-close!
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  const char *mode = CHAR(STRING_ELT(mode_, 0));
  strncpy(con->mode, mode, 4);
  con->mode[4] = '\0';
  if (strlen(mode) > 0) {
    con->open(con);
  }
  
  UNPROTECT(1);
  return rc;
}













