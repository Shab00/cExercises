# C Programming Exercises & Learning Journey

This repository documents my journey learning C through exercises, projects, and experiments. It contains solutions, starter templates, and small projects created while studying multiple books and resources.

Current focus: I am starting The C Programming Language (Kernighan & Ritchie) in addition to the earlier Head First C work. Expect new folders and exercises from K&R alongside existing Head First and Zed Shaw material.

Why this repo
- Practice core C fundamentals with small, focused exercises.
- Keep a history of experiments, false starts, and learning notes.
- Maintain lightweight, Makefile-driven projects to practice manual build workflows.
- Improve editing skills (Vim now, planning to move to Neovim later).

Learning resources
- Head First C — my practical, hands-on resource.
- The C Programming Language (K&R) — now starting; expect exercises and notes from this book.
- Learn C the Hard Way (Zed Shaw) — earlier exercises remain in the repo.
- c.org — supplementary reference.

Repository layout (high-level)
- cBookEx/ — exercises organized by book
  - theCProgramingLanguage/ — (K&R exercises; new and growing)
  - cHeadFirst/ — Head First C exercises and experiments
  - cZedShaw/ — Learn C the Hard Way exercises and liblcthw
    - c-skeleton/ — starter templates used in Zed's exercises (bin/, src/, tests/)
    - liblcthw/ — library code and tests used in Zed's projects
- cProjects/ — small full projects and utilities
  - blastoids/ (include/, src/)
  - plantTalk/ (git repo, experimental app)
  - calculatingCalories/ (bin/, include/, lib/, src/)
  - intruderDetector/ (bin/, include/, src/)
- playGround/ — experimental code, quick tests, and one-off files (e.g., ex24.dSYM)

Development environment
- Editor: Vim (working toward Neovim later)
- Build: Makefile-based per-exercise/project
- OS: macOS

Quick start — browsing the repo
- Print top-level directories only:
  find . -maxdepth 1 -type d -mindepth 1 -printf '%f\n'
- Print all directories recursively (excluding hidden dirs):
  find . -type d -not -path '*/\.*' -print
- If a project has a Makefile:
  cd path/to/project
  make
  # Then run the produced binary (usually in bin/ or project root)

How I use this repo
- Work through book chapters and create a folder per chapter or exercise.
- Add tests and a small Makefile when an exercise grows beyond a single file.
- Keep experimental code in playGround so core exercises remain tidy.
- Preserve older Zed Shaw exercises for reference while focusing on K&R and Head First C.

Contributing / Personal notes
- This repo is primarily a personal learning log; pulls or edits are welcome if you keep changes small and explain them.
- I may re-organize folders as I work through K&R — expect some renames/moves.
- If you clone and work locally, avoid committing large OS-specific artifacts (e.g., *.dSYM). You can add them to .gitignore.
