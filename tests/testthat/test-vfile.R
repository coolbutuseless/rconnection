
test_that("vfile works", {
  tmp <- tempfile()
  ref <- as.raw(1:255)
  writeBin(ref, vfile(tmp, verbosity = 0))
  tst <- readBin(vfile(tmp, verbosity = 0),  raw(), 1000)
  expect_identical(tst, ref)
  
  # ref <- as.character(mtcars)
  # writeLines(ref, vfile(tmp, verbosity = 0))
  # tst <- readLines(vfile(tmp, verbosity = 0))
  # expect_identical(tst, ref)
})