LP-Solver
=========

Linear program solver using the simplex method.

The program will first create the auxiliary problem, solve it, and if the optimal solution is 0, will solve the original program.  It will write down all the steps in the way as well. 


Usage
------------------
Have an LP in SEF in a text file like so:

sample.txt:

 max (-2 3 -1/2 4)x 
 (0 1 1 -1/3)x = 12
 (2 1 -1 -2)x = 10
 (0 -1 0 3)x = -2

run:

`$ make`
`$ ./Solver sample.txt`

Sample output:

  Original linear program is:
  max (   -2     3  -1/2     4 )x
      |    0     1     1  -1/3 |   = 12
      |    2     1    -1    -2 |x  = 10
      |    0    -1     0     3 |   = -2
  
  Canonical for basis ( 5  6  7 ) is:
  max (    2     1     0   2/3     0     0     0 )x - -20
      |    0     1     1  -1/3     1     0     0 |   = 12
      |    2     1    -1    -2     0     1     0 |x  = 10
      |    0    -1     0     3     0     0     1 |   = -2
  
  
  toleave is 6
  entering is 1
  Canonical for basis ( 1  5  7 ) is:
  max (    0     0     1   8/3     0    -1     0 )x - -10
      |    1   1/2  -1/2    -1     0   1/2     0 |   = 5
      |    0     1     1  -1/3     1     0     0 |x  = 12
      |    0    -1     0     3     0     0     1 |   = -2
  
  
  toleave is 5
  entering is 3
  Canonical for basis ( 1  3  7 ) is:
  max (    0    -1     0     3    -1    -1     0 )x + 2
      |    1     1     0  -7/6   1/2   1/2     0 |   = 11
      |    0     1     1  -1/3     1     0     0 |x  = 12
      |    0    -1     0     3     0     0     1 |   = -2
  
  
  toleave is 7
  entering is 4
  Canonical for basis ( 1  3  4 ) is:
  max (    0     0     0     0    -1    -1    -1 )x
      |    1   11/18     0     0   1/2   1/2   7/18 |   = 92/9
      |    0   8/9     1     0     1     0   1/9 |x  = 106/9
      |    0  -1/3     0     1     0     0   1/3 |   = -2/3
  
  
  Optimal solution: 
  |  92/9 |
  |    0 |
  |  106/9 |
  | -2/3 |
  |    0 |
  |    0 |
  |    0 |
  With optimal value: 0
  Canonical for basis ( 1  3  4 ) is:
  max (    0     6     0     0 )x - -29
      |    1   11/18     0     0 |   = 92/9
      |    0   8/9     1     0 |x  = 106/9
      |    0  -1/3     0     1 |   = -2/3
  
  
  toleave is 3
  entering is 2
  Canonical for basis ( 1  2  4 ) is:
  max (    0     0  -27/4     0 )x + 101/2
      |    1     0  -11/16     0 |   = 17/8
      |    0     1   9/8     0 |x  = 53/4
      |    0     0   3/8     1 |   = 15/4
  
  
  Optimal solution: 
  |  17/8 |
  |  53/4 |
  |    0 |
  |  15/4 |
  With optimal value: 101/2
