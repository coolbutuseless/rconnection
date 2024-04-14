
<!-- README.md is generated from README.Rmd. Please edit that file -->

# rconnection

<!-- badges: start -->

[![R-CMD-check](https://github.com/coolbutuseless/rconnection/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/coolbutuseless/rconnection/actions/workflows/R-CMD-check.yaml)
<!-- badges: end -->

`rconnection` demonstrates how to implement a custom connection in R.

This custom connection (`vfile()`) works similarly to the `file()`
connection, and all code for this implementation is included in this
repository.

This code is licensed under the MIT License - feel free to share, fork
and hack!

## Installation

You can install the development version of rconnection from
[GitHub](https://github.com/coolbutuseless/rconnection) with:

``` r
# install.packages("devtools")
devtools::install_github("coolbutuseless/rconnection")
```

## Using `vfile()` to read/write binary data

``` r
library(rconnection)

tmp <- tempfile()
ref <- as.raw(1:255)
writeBin(ref, vfile(tmp, verbosity = 1))
#> vfile_open('/tmp/RtmpQzP84j/file7e529e10ccd', mode = 'wb')
#> vfile_write(size = 1, nitems = 255)
#> vfile_close('/tmp/RtmpQzP84j/file7e529e10ccd')
tst <- readBin(vfile(tmp, verbosity = 1),  raw(), 1000)
#> vfile_open('/tmp/RtmpQzP84j/file7e529e10ccd', mode = 'rb')
#> vfile_read(size = 1, nitems = 1000)
#> vfile_close('/tmp/RtmpQzP84j/file7e529e10ccd')
tst
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
```

## Using `vfile()` to read/write text data

``` r
ref <- as.character(mtcars)
writeLines(ref, vfile(tmp, verbosity = 1))
#> vfile_open('/tmp/RtmpQzP84j/file7e529e10ccd', mode = 'wt')
#> vfile_vfprintf(fmt = '%s%s')
#> vfile_vfprintf(fmt = '%s%s')
#> vfile_vfprintf(fmt = '%s%s')
#> vfile_vfprintf(fmt = '%s%s')
#> vfile_vfprintf(fmt = '%s%s')
#> vfile_vfprintf(fmt = '%s%s')
#> vfile_vfprintf(fmt = '%s%s')
#> vfile_vfprintf(fmt = '%s%s')
#> vfile_vfprintf(fmt = '%s%s')
#> vfile_vfprintf(fmt = '%s%s')
#> vfile_vfprintf(fmt = '%s%s')
#> vfile_close('/tmp/RtmpQzP84j/file7e529e10ccd')
tst <- readLines(vfile(tmp, verbosity = 1))
#> vfile_open('/tmp/RtmpQzP84j/file7e529e10ccd', mode = 'rt')
#> vfile_close('/tmp/RtmpQzP84j/file7e529e10ccd')
tst
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
```
