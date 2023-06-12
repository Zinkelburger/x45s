// Copyright Andrew Bernal 2023
#pragma once
// https://learn.microsoft.com/en-us/cpp/cpp/extern-cpp?view=msvc-170
// Global variables may be bad, but trump is shared across all of the files
// main is the only one who should change trump and suitLed
extern int trump;
extern int suitLed;
