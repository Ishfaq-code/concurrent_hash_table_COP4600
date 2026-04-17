# How To Run
1. In the project directory, run `make`
2. Then run the executable by doing `./chash`

# AI USAGE

Chat GPT's model Codex 5.3 was used to do create the portion of reading through the `commands.txt` file found in `chash.c`. I did so because it is an unreleated task which was ought to be automated as we are focused on the concurrency aspect of the project, not actual file IO. The model was used through the Opencode TUI which allows for the model to have full context of the codebase allowing for precise implementation.

This is how the interaction was:

Prompt (In Plan Mode): "Please plan out a way to read from a file named "commands.txt" which look's as the following: **COMMANDS.TXT PASTED**. The first line should be reading and creating the threads. The next few lines should be doing the operations.

In plan mode (which is part of Opencode) it allows for the model to go through the codebase and make a plan of the implementation without editing the codebase. After making the plan, I put into build mode which designed the skelton of the file reading system.

Chat GPT's model Codex 5.3 also used to for `Makefile` of this project. This is to properly create and link the files during execution. I did not have much experience with actual project infrasturcture for a language such as C, so I utilized AI to create this.

Prompt (In Plan Mode): "Please plan out a Makefile for this project. I am writing a multi-file project in C. Show me the file and tell me how it works"