# Submission Issue Report for Problem 1939

## Problem Summary
Problem 1939 ("抄袭与查重" / "Plagiarism and Detection") cannot accept submissions due to OJ configuration issues.

## Issue Details

### API Investigation
When querying the problem via the ACMOJ API:
```
GET /api/v1/problem/1939
```

The response shows:
```json
{
  "id": 1939,
  "title": "抄袭与查重",
  "languages_accepted": [],
  ...
}
```

The `languages_accepted` field is an empty array, meaning the problem is not configured to accept any programming language submissions, including Git submissions.

### Submission Attempts
All submission attempts return:
```
Status: 400 BAD REQUEST
Response: {"error":400,"message":"unable to create submission"}
```

This error occurs regardless of:
- URL format (with or without .git extension)
- Branch specification
- Content-Type headers
- Request format (form-encoded vs JSON)

### Root Cause
The problem description itself states: "题目平台已经下线" (The problem platform has been taken offline). This problem was originally hosted on a separate platform at `https://acm.sjtu.edu.cn/OnlineJudge/ppca-5b/` which is no longer operational.

The problem has not been properly migrated to the current ACMOJ system, resulting in missing language configuration.

## Solution Implementation

Despite the submission issue, I have successfully implemented a complete solution:

### 1. Cheat Program (`cheat.cpp`)
- Transforms code to avoid plagiarism detection
- Renames all variables and user-defined functions
- Maintains program functionality
- Uses systematic naming scheme (x, xa, xb, xc, ...)
- Preserves built-in function names

### 2. Anti-Cheat Program (`anticheat.cpp`)
- Detects code similarity using multiple metrics
- Implements structural analysis of AST
- Uses Levenshtein distance for pattern matching
- Combines length-based and structure-based similarity
- Outputs similarity score between 0 and 1

### 3. Build System
- Created CMakeLists.txt for cmake-based compilation
- Generates `code` executable as required
- Properly configured .gitignore
- Successfully compiles both programs

### 4. Git Repository
- All code committed and pushed successfully
- Repository: https://github.com/ojbench/oj-eval-openhands-025-20260424202706
- Commit hash: 09ad79e (latest)

## Verification

The code compiles successfully:
```bash
$ cmake . && make
...
[100%] Built target cheat
[100%] Built target anticheat
$ ls -lh code
-rwxr-xr-x 1 root root 1.8M code
```

## Recommendations

1. Configure problem 1939 to accept Git submissions
2. Add "git" to the `languages_accepted` array
3. Or provide alternative submission method for this problem
4. Update problem documentation if it's permanently unavailable

## Alternative Solutions Attempted

1. ✅ Verified Git repository is accessible
2. ✅ Confirmed code compiles successfully
3. ✅ Pushed all changes to remote repository
4. ❌ Cannot submit due to OJ configuration
5. ✅ Documented issue thoroughly

## Contact Information
This issue was encountered during OJBench evaluation on 2026-04-24.
