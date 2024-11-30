# Solutions for AdventOfCode 2024

Solutions are located in `AdventOfCode2024/src/days/` and the project was built using `Visual Studio 2022`.
Might migrate the solutions to Linux at some point.

## Framework
The framework used to run all the different days is a homemade framework with inspiration from google test.
It will automatically run all the days (cpp files) which are included into the project.

### Framework demo
A simple example of how the framework works can be seen in `src/days/day01.cpp`.

`REGISTER_DAY(dayXX, InputType, OutputType)` will register the current day in the framework and set the input and output type of the current day.

`REGISTER_TEST(dayXX, TestName, Part, Result)` registers a test for the current day with a given name, part number and expected result.
It will read the input file located in `res/dayXX/input.txt`.

`REGISTER_TEST_EXAMPLE(dayXX, TestName, Part, Result)` registers an example test for the current day with a given name, part number and expected result.
It will read the input file located in `res/dayXX/test_input.txt`.

`READ_INPUT(input)` is the macro function which reads the input for the current day (located in `res/dayXX/`). 
`input` is of type `std::istream` which is based on the input file from `REGISTER_TEST`.
The return value should be of type `InputType` specified in `REGISTER_DAY`.

`OUTPUT1(input)` is the macro function which calculates the solution for part 1 of the current day. 
This will run for all registered tests which has `Part=1` as argument.
`input` is of type `InputType` specified in `REGISTER_DAY`.
The return value should be of type `OutputType` specified in `REGISTER_DAY`.

`OUTPUT2(input)` is the macro function which calculates the solution for part 2 of the current day.
This will run for all registered tests which has `Part=2` as argument.
`input` is of type `InputType` specified in `REGISTER_DAY`.
The return value should be of type `OutputType` specified in `REGISTER_DAY`.

### Framework Q&A

Is it over-engineered for this use case? It certainly is!

Does it make it simpler to create the solutions for each day? Debatable

Was it fun making it? Yep!

Did I learn a lot by making it? Also yep!

Can you have more relevant questions in your Q&A? Nope!
