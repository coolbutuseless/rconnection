
<!-- README.md is generated from README.Rmd. Please edit that file -->

# `{rconnection}`: a demonstration package implementing a custom connection in R

<!-- badges: start -->

[![R-CMD-check](https://github.com/coolbutuseless/rconnection/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/coolbutuseless/rconnection/actions/workflows/R-CMD-check.yaml)
<!-- badges: end -->

`{rconnection}` is a demonstration package implementing a custom
connection in R `vfile()`

(`vfile()`) works similarly to the `file()` connection except it is
verbose about which connection callback is being used internally, and it
has fewer features e.g. it does not support file append or seek etc.

This code is MIT Licensed - feel free to share, fork and hack!

### Suitability of this approach for CRAN

The code to create a connection causes a `NOTE` in `R CMD check` which
says: “Found non-API call to R: ‘R_new_custom_connection’. Compiled code
should not call non-API entry points in R.”

As this call to `R_new_connection()` exists in R packages on CRAN, this
NOTE has been allowed through by CRAN team on some occasions. Your
results may vary.

## Installation

You can install the development version of rconnection from
[GitHub](https://github.com/coolbutuseless/rconnection) with:

``` r
# install.packages("devtools")
devtools::install_github("coolbutuseless/rconnection")
```

## Using `vfile()` to read/write binary data

Write binary data to a `vfile()` connection and then read it back in.

``` r
library(rconnection)

tmp <- tempfile()
ref <- as.raw(1:255)
writeBin(ref, vfile(tmp, verbosity = 1))
tst <- readBin(vfile(tmp, verbosity = 1),  raw(), 1000)
tst
```

``` r
library(rconnection)

tmp <- tempfile()
ref <- as.raw(1:255)
writeBin(ref, vfile(tmp, verbosity = 1))
```

    #> vfile_open('/var/folders/kq/h7dv19mj00947dthlyb5w2780000gn/T//RtmpLJVICU/file169308b4f0df', mode = 'wb')
    #> vfile_write(size = 1, nitems = 255)
    #> vfile_close('/var/folders/kq/h7dv19mj00947dthlyb5w2780000gn/T//RtmpLJVICU/file169308b4f0df')

``` r
tst <- readBin(vfile(tmp, verbosity = 1),  raw(), 1000)
```

    #> vfile_open('/private/var/folders/kq/h7dv19mj00947dthlyb5w2780000gn/T/RtmpLJVICU/file169308b4f0df', mode = 'rb')
    #> vfile_read(size = 1, nitems = 1000)
    #> vfile_close('/private/var/folders/kq/h7dv19mj00947dthlyb5w2780000gn/T/RtmpLJVICU/file169308b4f0df')

``` r
tst
```

    #>   [1] 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16 17 18 19
    #>  [26] 1a 1b 1c 1d 1e 1f 20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f 30 31 32
    #>  [51] 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f 40 41 42 43 44 45 46 47 48 49 4a 4b
    #>  [76] 4c 4d 4e 4f 50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f 60 61 62 63 64
    #> [101] 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d
    #> [126] 7e 7f 80 81 82 83 84 85 86 87 88 89 8a 8b 8c 8d 8e 8f 90 91 92 93 94 95 96
    #> [151] 97 98 99 9a 9b 9c 9d 9e 9f a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 aa ab ac ad ae af
    #> [176] b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 ba bb bc bd be bf c0 c1 c2 c3 c4 c5 c6 c7 c8
    #> [201] c9 ca cb cc cd ce cf d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 da db dc dd de df e0 e1
    #> [226] e2 e3 e4 e5 e6 e7 e8 e9 ea eb ec ed ee ef f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa
    #> [251] fb fc fd fe ff

## Using `vfile()` to read/write text data

Write text to a `vfile()` connetion and then read it back in.

``` r
ref <- as.character(mtcars)
writeLines(ref, vfile(tmp, verbosity = 1))
tst <- readLines(vfile(tmp, verbosity = 1))
tst
```

``` r
ref <- as.character(mtcars)
writeLines(ref, vfile(tmp, verbosity = 1))
```

    #> vfile_open('/private/var/folders/kq/h7dv19mj00947dthlyb5w2780000gn/T/RtmpLJVICU/file169308b4f0df', mode = 'wt')
    #> vfile_vfprintf('c(21, 21, 22.8, 21.4, 18.7, 18.1, 14.3,  ...')
    #> vfile_vfprintf('c(6, 6, 4, 6, 8, 6, 8, 4, 4, 6, 6, 8, 8, ...')
    #> vfile_vfprintf('c(160, 160, 108, 258, 360, 225, 360, 146 ...')
    #> vfile_vfprintf('c(110, 110, 93, 110, 175, 105, 245, 62,  ...')
    #> vfile_vfprintf('c(3.9, 3.9, 3.85, 3.08, 3.15, 2.76, 3.21 ...')
    #> vfile_vfprintf('c(2.62, 2.875, 2.32, 3.215, 3.44, 3.46,  ...')
    #> vfile_vfprintf('c(16.46, 17.02, 18.61, 19.44, 17.02, 20. ...')
    #> vfile_vfprintf('c(0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, ...')
    #> vfile_vfprintf('c(1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ...')
    #> vfile_vfprintf('c(4, 4, 4, 3, 3, 3, 3, 4, 4, 4, 4, 3, 3, ...')
    #> vfile_vfprintf('c(4, 4, 1, 1, 2, 1, 4, 2, 2, 4, 4, 3, 3, ...')
    #> vfile_close('/private/var/folders/kq/h7dv19mj00947dthlyb5w2780000gn/T/RtmpLJVICU/file169308b4f0df')

``` r
tst <- readLines(vfile(tmp, verbosity = 1))
```

    #> vfile_open('/private/var/folders/kq/h7dv19mj00947dthlyb5w2780000gn/T/RtmpLJVICU/file169308b4f0df', mode = 'rt')
    #> vfile_close('/private/var/folders/kq/h7dv19mj00947dthlyb5w2780000gn/T/RtmpLJVICU/file169308b4f0df')

``` r
tst
```

    #>  [1] "c(21, 21, 22.8, 21.4, 18.7, 18.1, 14.3, 24.4, 22.8, 19.2, 17.8, 16.4, 17.3, 15.2, 10.4, 10.4, 14.7, 32.4, 30.4, 33.9, 21.5, 15.5, 15.2, 13.3, 19.2, 27.3, 26, 30.4, 15.8, 19.7, 15, 21.4)"                    
    #>  [2] "c(6, 6, 4, 6, 8, 6, 8, 4, 4, 6, 6, 8, 8, 8, 8, 8, 8, 4, 4, 4, 4, 8, 8, 8, 8, 4, 4, 4, 8, 6, 8, 4)"                                                                                                            
    #>  [3] "c(160, 160, 108, 258, 360, 225, 360, 146.7, 140.8, 167.6, 167.6, 275.8, 275.8, 275.8, 472, 460, 440, 78.7, 75.7, 71.1, 120.1, 318, 304, 350, 400, 79, 120.3, 95.1, 351, 145, 301, 121)"                       
    #>  [4] "c(110, 110, 93, 110, 175, 105, 245, 62, 95, 123, 123, 180, 180, 180, 205, 215, 230, 66, 52, 65, 97, 150, 150, 245, 175, 66, 91, 113, 264, 175, 335, 109)"                                                     
    #>  [5] "c(3.9, 3.9, 3.85, 3.08, 3.15, 2.76, 3.21, 3.69, 3.92, 3.92, 3.92, 3.07, 3.07, 3.07, 2.93, 3, 3.23, 4.08, 4.93, 4.22, 3.7, 2.76, 3.15, 3.73, 3.08, 4.08, 4.43, 3.77, 4.22, 3.62, 3.54, 4.11)"                  
    #>  [6] "c(2.62, 2.875, 2.32, 3.215, 3.44, 3.46, 3.57, 3.19, 3.15, 3.44, 3.44, 4.07, 3.73, 3.78, 5.25, 5.424, 5.345, 2.2, 1.615, 1.835, 2.465, 3.52, 3.435, 3.84, 3.845, 1.935, 2.14, 1.513, 3.17, 2.77, 3.57, 2.78)"  
    #>  [7] "c(16.46, 17.02, 18.61, 19.44, 17.02, 20.22, 15.84, 20, 22.9, 18.3, 18.9, 17.4, 17.6, 18, 17.98, 17.82, 17.42, 19.47, 18.52, 19.9, 20.01, 16.87, 17.3, 15.41, 17.05, 18.9, 16.7, 16.9, 14.5, 15.5, 14.6, 18.6)"
    #>  [8] "c(0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1)"                                                                                                            
    #>  [9] "c(1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1)"                                                                                                            
    #> [10] "c(4, 4, 4, 3, 3, 3, 3, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 4, 4, 4, 3, 3, 3, 3, 3, 4, 5, 5, 5, 5, 5, 4)"                                                                                                            
    #> [11] "c(4, 4, 1, 1, 2, 1, 4, 2, 2, 4, 4, 3, 3, 3, 4, 4, 4, 1, 2, 1, 1, 2, 2, 4, 2, 1, 2, 2, 4, 6, 8, 2)"

## What is an R connection?

From [Jeroen Ooms on
StackOverflow](https://stackoverflow.com/questions/30445875/what-exactly-is-a-connection-in-r)

    Connections were introduced in R 1.2.0 and described by Brian Ripley in
    the first issue of R NEWS (now called The R Journal) of  January 2001 (page 16-17) (Ref: 1)
    as an abstracted interface to IO streams such as a file, url, socket, or 
    pipe. In 2013, Simon Urbanek added a Connections.h C API (Ref: 2)
    which enables R packages to implement custom connection types, such as the 
    curl package.

    One feature of connections is that you can incrementally read or write 
    pieces of data from/to the connection using the readBin, writeBin, 
    readLines and writeLines functions. This allows for asynchronous data 
    processing, for example when dealing with large data or network 
    connections:

1.  [January 2001 (page
    16-17)](http://cran.r-project.org/doc/Rnews/Rnews_2001-1.pdf)
2.  \[[Connections.h C
    API](https://github.com/wch/r-source/blob/trunk/src/include/R_ext/Connections.h)\]

## R Packages which implement connections

- `{rconnection}` This package! [rconnection on
  Github](https://github.com/coolbutuseless/rconnection)
- [zstdlite](https://github.com/coolbutuseless/zstdlite) implements a
  `zstdfile()` connection for read/write of compressed data
- [rmonocypher](https://github.com/coolbutuseless/rmonocypher)
  implements a `cryptfile()` connection for read/write of encypted data
- [archive](https://cran.r-project.org/package=archive)
- [curl](https://cran.r-project.org/package=curl)
- And base R implements: `gzfile()`, `bzfile()`, `xzfile()`, `file()`,
  `url()`, `unz()`, `pipe()`, `fifo()`, `socketConnection()`,
  `textConnection()`

## Base R functions supporting connections

`serialize()`, `saveRDS()`, `readRDS()`, `cat()`, `sink()`,
`writeChar()`, `readChar()`, `read.csv()`, `read.delim()`, and many
more.

## Documentation and write-ups on R connections

- Matthew S Shotwell’s ‘R Connection Internals’
  - [html](https://biostatmatt.com/R/R-conn-ints/)
  - [pdf](http://biostatmatt.com/R/R-conn-ints.pdf)

## Connection calls may be nested

Some connection objects can themselves read/write to connections e.g.
base R’s `gzcon()` or the `vfile()` connection in this package.

In the following example, the `vfile()` connection will write data to a
`gzfile()` connection.

``` r
tmp <- tempfile()
writeBin(as.raw(1:10), vfile(gzfile(tmp)))
```

    #> vfile_open('vfile(connection)', mode = 'wb')
    #> vfile_write(size = 1, nitems = 10)
    #> vfile_close('vfile(connection)')

Connection nesting could be useful for niche ideas like “write
compressed, emcrypted data to a socket” using:

- `zstdlite::zstdfile()` for compression
- `rmonocypher::cryptfile()` for encryption
- `socketConnection()` for writing to a socket

``` r
saveRDS(mtcars, zstdfile(cryptfile(socketConnection(...), key = "my secret")))
```

## The `Rconn` data structure

From Matthew Shotwell’s documentation of the core internal R connection
structure `Rconn` copied from [his
website](https://biostatmatt.com/R/R-conn-ints/C-Structures.html#C-Structures)
and included here for posterity.

    Below are the definitions for struct Rconn and struct fileconn copied from 
    src/include/Rconnections.h. Additional comments by this author (Mattheew)
    are inserted between special delimiters of the form /** comment **/. 

    struct Rconn {
        /** class name (null terminated) **/
        char* class;

        /** description (null terminated), can be a filename, url, or other 
            identifier, depending on the connection type 
        **/
        char* description;
        int enc; /* the encoding of 'description' */

        /** file operation mode (null terminated) **/
        char mode[5];

        /** text       - true if connection operates on text
            isopen     - true if connection is open
            incomplete - used in @code{do_readLines}, @code{do_isincomplete}, 
                         and text_vfprintf, From `?connections`: true if last 
                         read was blocked, or for an output text connection whether 
                         there is unflushed output
            canread    - true if connection is readable
            canwrite   - true if connection is writable
            canseek    - true if connection is seekable
            blocking   - true if connection reads are blocking
            isGzcon    - true if connection operates on gzip compressed data 
        **/
        Rboolean text, isopen, incomplete, canread, canwrite, canseek, blocking, 
        isGzcon;

        /** function pointers for I/O operations **/


        /** open - called when the connection should be opened
            args: struct Rconn * - an initialized connection to be opened
            return: Rboolean - true if connection successfully opened, false otherwise
        **/
        Rboolean (*open)(struct Rconn *);


        /** close - called when the connection should be closed
            args: struct Rconn * - a connection to be closed
        **/
        void (*close)(struct Rconn *); /* routine closing after auto open */


        /** destroy - called after the connection is closed in order to free memory, 
            and other cleanup tasks
            args: struct Rconn * - a connection to be closed
        **/
        void (*destroy)(struct Rconn *); /* when closing connection */


        /** vfprintf - variable argument list version of printf for a connection
            args: struct Rconn * - a connection where items should be printed
                  const char *   - a format string in the style of the printf family
                  va_list        - a variable argument list containing the items 
                                   referred to in the format string
            return: int - number of characters printed, negative on failure
        **/
        int (*vfprintf)(struct Rconn *, const char *, va_list);


     
        /** fgetc - get a (re-encoded) character from the connection
            args: struct Rconn * - a connection to be read
            return: int - a (re-encoded) character, or R_EOF
        **/
        int (*fgetc)(struct Rconn *);


        /** fgetc_internal - get a character from the connection
            args: struct Rconn * - a connection to be read
            return: int - a character, or R_EOF
        **/
        int (*fgetc_internal)(struct Rconn *);


        /** seek - seek to a new position in the connection
            args: struct Rconn * - a connection to seek
                  double         - offset to seek relative to origin, apparently 
                                   double is used here to avoid using 
                                   integer types, i.e. long int, which is 
                                   the prototype of the corresponding parameter 
                                   in fseek, as defined in stdio.h
                  int            - the origin of seeking, 1 (and any except 2 and
                                   3) if relative to the beginning of the 
                                   connection, 2 if relative to the current 
                                   connection read/write position, 3 if relative to 
                                   the end of the connection
                  int            - currently only used by file_seek to select 
                                   the read or write position when the offset is NA
            return: double - the read/write position of the connection before 
                             seeking, negative on error double is again used to 
                             avoid integer types
        **/
        double (*seek)(struct Rconn *, double, int, int);


        /** truncate - truncate the connection at the current read/write position.
            args: struct Rconn * - a connection to be truncated
        **/
        void (*truncate)(struct Rconn *);


        /** fflush - called when the connection should flush internal read/write buffers
            args: struct Rconn * - a connection to be flushed
            return: int - zero on success, non-zero otherwise
        **/
        int (*fflush)(struct Rconn *);


        /** read - read in the style of fread
            args: void *         - buffer where data is read into
                  size_t         - size (in bytes) of each item to be read
                  size_t         - number of items to be read
                  struct Rconn * - a connection to be read
            return: size_t - number of _items_ read
        **/
        size_t (*read)(void *, size_t, size_t, struct Rconn *);


        /** write - write in the style of fwrite
            args: void *         - buffer containing data to be written
                  size_t         - size (in bytes) of each item to be written
                  size_t         - number of items to be written
                  struct Rconn * - a connection to be written
            return: size_t - number of _items_ written
        **/
        size_t (*write)(const void *, size_t, size_t, struct Rconn *);
       
        /** cached and pushBack data
            nPushBack   - number of lines of cached/pushBack storage
            posPushBack - read position on current line of storage
            PushBack    - cached/pushBack data lines ('\n' delimited)
            save        - used to store the character following a \n, if not \r
            save2       - used to store a character from Rconn_ungetc
        **/
        int nPushBack, posPushBack; /* number of lines, position on top line */
        char **PushBack;
        int save, save2;

        /** character re-encoding with iconv
            encname   - character encoding string (null terminated), this string 
                        must be one of the standard encoding strings used by [lib]iconv
            inconv    - input character encoding context (iconv_t)
            outconv   - output character encoding context (iconv_t)
            iconvbuff - input character encoding buffer
            oconvbuff - output character encoding buffer
            next      - only used by dummy_fgetc, points to the next re-encoded 
                        character for reading
            init_out  - storage for output iconv initialization sequence 
            navail    - iconv buffer offset
            inavail   - iconv buffer offset
            EOF_signalled - true if EOF reached
            UTF8out   - true if connection writes UTF8 encoded characters
        **/
        char encname[101];
        /* will be iconv_t, which is a pointer. NULL if not in use */
        void *inconv, *outconv;
        /* The idea here is that no MBCS char will ever not fit */
        char iconvbuff[25], oconvbuff[50], *next, init_out[25];
        short navail, inavail;
        Rboolean EOF_signalled;
        Rboolean UTF8out;

        /** finalization pointers
            id     - unique id, used to "ensure that the finalizer does not 
                     try to close connection after it is alread closed"
                     (quoted from source code), but also to identify the
                     connection to be finalized. Using an arbitrary but
                     unique id here is clever, it means the connections 
                     internals are further protected from passing references 
                     to connection structures.
            ex_ptr - external pointer, referenced by finalizer code
        **/
        void *id;
        void *ex_ptr;

        /** private user data (i.e. FILE *, offsets etc.) **/
        void *private;
    };
