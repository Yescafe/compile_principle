CC      := clang++
CCFLAGS := -std=c++20
EXEC    := word_analysis syntax_parser

prj: ${EXEC}

word_analysis: word_analysis.cc
	${CC} word_analysis.cc -o word_analysis ${CCFLAGS}

syntax_parser: syntax_parser.cc
	${CC} syntax_parser.cc -o syntax_parser ${CCFLAGS}

.PHONY:
clean:
	rm ${EXEC}
