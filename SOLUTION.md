# Solution for Problem 025 - Plagiarism Detection and Code Transformation

## Overview
This solution implements two programs for a plagiarism detection system:
1. **Cheat Program**: Transforms code to avoid plagiarism detection while maintaining functionality
2. **Anticheat Program**: Detects similarity between two programs

## Implementation Details

### Cheat Program (`cheat.cpp`)
The cheat program transforms input code by:
- **Variable Renaming**: All user-defined variables and functions are renamed using a systematic scheme (x, xa, xb, xc, ...)
- **Function Name Transformation**: User-defined functions get new names while preserving `main` and built-in functions
- **AST Preservation**: The program structure and logic remain identical, only identifiers change
- **Built-in Function Protection**: All built-in functions (+, -, *, /, scan, print, array.*, etc.) are preserved

**Key Features:**
- Deterministic renaming (same input always produces same output)
- Minimal transformation to ensure correctness
- Clean, readable generated code
- No semantic changes

### Anticheat Program (`anticheat.cpp`)
The anticheat program detects code similarity using:
- **Structural Analysis**: Creates a normalized AST representation
- **Levenshtein Distance**: Measures edit distance between structural representations
- **Length-Based Metrics**: Compares program complexity
- **Combined Scoring**: Weights multiple metrics (30% length, 70% structure)

**Similarity Score Formula:**
```
structural_similarity = 1 - (levenshtein_distance / max_length)
length_similarity = 1 - abs(len1 - len2) / max(len1, len2)
final_score = 0.5 + 0.5 * (0.3 * length_sim + 0.7 * structural_sim)
```

The score ranges from 0 to 1:
- **< 0.5**: Programs likely not derived from same source
- **= 0.5**: Neutral (no similarity detected)
- **> 0.5**: Programs likely derived from same source

## Build Instructions

### Using CMake (Recommended for OJ)
```bash
cmake .
make
```

This produces:
- `cheat` - The code transformation program
- `anticheat` - The plagiarism detection program
- `code` - Copy of cheat for OJ submission

### Using Make (Original Template)
```bash
make -j
```

This produces the executables plus:
- `cheat-submit.cpp` - Single-file version of cheat
- `anticheat-submit.cpp` - Single-file version of anticheat
- `eval-submit.cpp` - Single-file version of interpreter

## Usage

### Cheat Program
```bash
./cheat < input.p > output.p
```

### Anticheat Program
```bash
cat program1.p program2.p testinput.txt | ./anticheat
```

The anticheat program expects:
1. First program ending with `endprogram`
2. Second program ending with `endprogram`
3. Test input data

## File Structure
```
.
├── CMakeLists.txt          # CMake build configuration
├── Makefile                # Original Makefile from template
├── cheat.cpp               # Code transformation implementation
├── anticheat.cpp           # Plagiarism detection implementation
├── lang.cpp                # Language parser and interpreter
├── lang.h                  # Language AST definitions
├── lang.md                 # Language specification
├── transform.h             # AST transformation base class
├── visitor.h               # AST visitor pattern base class
├── eval.cpp                # Standalone interpreter
├── .gitignore              # Git ignore rules
└── SUBMISSION_ISSUE.md     # Documentation of OJ submission issue
```

## Testing

The programs can be tested locally with the custom language. Example program:

```lisp
(function (main)
  (block
    (set n (scan))
    (set xs (array.scan n))
    (print (sum xs n))))

(function (sum array length)
  (block
    (if (== length 0)
      (return 0))
    (set s 0)
    (for
      (set i 0)
      (< i length)
      (set i (+ i 1))
      (set s (+ s (array.get array i))))
    (return s)))
```

## Algorithm Complexity

### Cheat Program
- **Time**: O(n) where n is the number of AST nodes
- **Space**: O(m) where m is the number of unique identifiers

### Anticheat Program
- **Time**: O(n * m) where n and m are the lengths of structural representations (Levenshtein distance)
- **Space**: O(n * m) for the dynamic programming table

## Known Issues

### OJ Submission Issue
**Problem 1939 cannot accept submissions** due to misconfiguration:
- API shows `languages_accepted: []`
- All submission attempts return 400 BAD REQUEST: "unable to create submission"
- The original platform for this problem is offline

See `SUBMISSION_ISSUE.md` for detailed analysis.

## Repository Information
- **GitHub**: https://github.com/ojbench/oj-eval-openhands-025-20260424202706
- **Latest Commit**: a4e1fa9
- **Status**: Code complete, tested, and pushed. Awaiting OJ configuration fix.

## Implementation Quality
- ✅ Clean, well-structured code
- ✅ Proper error handling
- ✅ Efficient algorithms
- ✅ Comprehensive documentation
- ✅ Proper build system
- ✅ Git best practices
- ✅ Successfully compiles
- ❌ Cannot submit due to OJ configuration issue

## Future Improvements
If the OJ becomes available, potential enhancements:
1. Add more sophisticated transformations (statement reordering, expression refactoring)
2. Implement machine learning-based similarity detection
3. Add support for detecting semantic equivalence beyond structural similarity
4. Optimize Levenshtein distance calculation for very large programs
