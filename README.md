# hashcode

Building all execulables:

```
./build.sh
```

Re-building your executable:
```
./build.sh {lexa/timur/danya/dima}
```

Running your solution on specific test:

```
./run.sh {lexa/timur/danya/dima} {a/b/c/...} [-l|--long]
```

Running your solution on all tests:

```
./run.sh {lexa/timur/danya/dima} [-l|--long]
```

By default your results are stored in subfolder `results/{lexa/timur/...}`.
Use `[-l|--long]` flag for computationaly-expensive long runs (>1 min). It will store results in separate subfolder with unique name, so you won't be able to override your results by accident.

Following data stored is stored:
* Your binary `{lexa/timur/danya/dima}` which will actually be used for running
* Tar file with your `*.cpp` and `common.h`
* `*.ans` files for the solution
