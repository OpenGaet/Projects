TITLE Composite Numbers     (Project04.asm)

; Author: Gaetan Ingrassia
; Last Modified: 2019-02-17
; OSU email address: ingrassg@oregonstate.edu
; Course number/section: CS271-400
; Project Number: #04                Due Date: 2019-02-17
; Description: This program asks the user for the number of composite numbers they want to
;	have calculated and displayed. These numbers are then displayed in rows with 10 columns.

INCLUDE Irvine32.inc

UPPER_BOUND			EQU				400

.data

intro				BYTE			"	  Composite Number Fun   by Gaetan Ingrassia",0
extraCred1			BYTE			"**EC: Displays numbers in aligned columns.",0
instruct1			BYTE			"Please enter the number of composite numbers you would like to see.",0
instruct2			BYTE			"I can only work in the range [1, 400].",0
prompt				BYTE			"Enter the number of composites to display [1, 400]: ",0
invalidNum			BYTE			"That number is out of range, try again.",0
numTerms			DWORD			?			; Number of terms to be displayed
compSeed			DWORD			4			; This is the first composite number
spacing				BYTE			"	",0
compLoopStart		DWORD			5			; Sets the starting value for the compCheck loop
col					DWORD			1			; Keeps track of current column
outro				BYTE			"I hope you enjoyed using this program! Peace and Love from Gaetan!",0

.code
main PROC

	call	introduction
	call	getUserData
	call	showComposites
	call	farewell

	exit	; exit to operating system
main ENDP

;---------------------------------------------------------------------------------
; introduction
;
; Introduces the program, its rules, and the programmer to the user
; Pre:		 NA
; Receives:	 The global variables intro, extraCred1, instruct1, instruct2
; Returns:	 EDX = OFFSET instruct2
; Registers: EDX 
;---------------------------------------------------------------------------------
introduction		PROC

	; Greeting 
	mov		edx, OFFSET intro
	call	WriteString
	call	CrLf
	call	CrLf
	; Extra credit messages
	mov		edx, OFFSET extraCred1
	call	WriteString
	call	CrLf
	call	CrLf
	; Instructions
	mov		edx, OFFSET instruct1
	call	WriteString
	call	CrLf
	mov		edx, OFFSET instruct2
	call	WriteString
	call	CrLf
	call	CrLf

	ret
introduction		ENDP

;---------------------------------------------------------------------------------
; getUserData
;
; Continually prompts user for number in range [1, 400] using a validation loop
; Pre:		 NA
; Receives:	 The global variables invalidNum, numTerms, prompt
; Returns:	 EAX = numTerms = user input
;			 EBX = 1 if valid input, 0 otherwise
;			 EDX = OFFSET prompt or OFFSET invalidNum
; Registers: EAX, EBX, EDX
;---------------------------------------------------------------------------------
getUserData			PROC

	; Prompt user, validate number, repeat if necessary
inputLoop:
	mov		edx, OFFSET prompt
	call	WriteString
	call	ReadInt
	mov		numTerms, eax
	call	validate
	cmp		ebx, 1
	je		inputLoopEnd
	mov		edx, OFFSET invalidNum
	call	WriteString
	call	CrLf
	jmp		inputLoop
inputLoopEnd:

	ret
getUserData			ENDP

;---------------------------------------------------------------------------------
; showComposites
;
; Displays the composite numbers with a max of 10 columns
; Pre:		 NA
; Receives:	 The global variables col, compSeed, numTerms, spacing
; Returns:	 EAX=EBX=EDX = registers returned from isComposite, ECX = 0
; Registers: EAX, EBX, ECX, EDX
;---------------------------------------------------------------------------------
showComposites		PROC

	mov		ecx, numTerms
compLoop:
	push	ecx
	mov		eax, compSeed
	call	WriteDec
	mov		edx, OFFSET spacing
	call	WriteString
	; Check column number per row
	inc		col
	cmp		col, 10
	jle		noWrite
	mov		col, 1
	call	CrLf
noWrite:
	inc		compSeed
	call	isComposite
	cmp		ebx, 1
	jnz		noWrite
	pop		ecx
	loop	compLoop

	ret
showComposites		ENDP

;---------------------------------------------------------------------------------
; farewell
;
; Outputs a farewell message to the user
; Pre:		 NA
; Receives:	 The global variable outro
; Returns:	 EDX = OFFSET outro
; Registers: EDX
;---------------------------------------------------------------------------------
farewell				PROC

	call	CrLf
	call	CrLf
	mov		edx, OFFSET outro
	call	WriteString
	call	CrLf

	ret
farewell				ENDP

;---------------------------------------------------------------------------------
; validate
;
; Sub-procedure of getUserData to validate user input is within range
; Pre:		 numTerms contains an integer to be validated
; Receives:	 numTerms and the global constant UPPER_BOUND
; Returns:	 EBX = 1 if valid number, or,
;			 EBX = 0 if invalid number		
; Registers: EAX, EBX
;---------------------------------------------------------------------------------
validate			PROC

	; Set return to 0, change to 1 if number given is within range
	mov		ebx, 0
	cmp		numTerms, 1
	jl		invalid
	mov		eax, UPPER_BOUND
	cmp		numTerms, eax
	jg		invalid
	mov		ebx, 1
invalid:

	ret
validate			ENDP

;---------------------------------------------------------------------------------
; isComposite
;
; Sub-Procedure of showComposites to determine if the number is a composite
; Pre:		 EAX contains the number to be tested for being composite
; Receives:	 EAX
; Returns:	 EBX = 1 if composite, or,
;			 EBX = 0 if not composite
; Registers: EAX, EBX, ECX, EDX
;---------------------------------------------------------------------------------
isComposite			PROC
	
	mov		ebx, 0
	; Check evens
	mov		ecx, 2
	call	divSeed
	cmp		edx, 0
	jz		compCheckEnd
	; Check multiples of three
	mov		ecx, 3
	call	divSeed
	cmp		edx, 0
	jz		compCheckEnd
	; Check for 5
	cmp		compSeed, 5
	jz		notComposite
	; Check for 7
	cmp		compSeed, 7
	jz		notComposite

	mov		ecx, compLoopStart
compCheck:
	; Check all other numbers
	call	divSeed
	cmp		edx, 0
	jz		compCheckEnd		
	add		ecx, 2
	call	divSeed
	cmp		edx, 0
	jz		compCheckEnd
	add		ecx, 4
	mov		eax, ecx
	mul		eax
	cmp		eax, compSeed
	jle		compCheck
	jmp		notComposite
compCheckEnd:
	mov		ebx, 1
notComposite:

	ret
isComposite			ENDP

;---------------------------------------------------------------------------------
; divSeed
;
; Sub-Procedure of isComposite to divide compSeed by ECX
; Pre:		 ECX contains the divisor
; Receives:	 The variable compSeed 
; Returns:	 EAX = The quotient
;			 EDX = The remainder
; Registers: EAX, EDX		
;---------------------------------------------------------------------------------
divSeed			PROC

	mov		eax, compSeed
	xor		edx, edx
	div		ecx

	ret
divSeed			ENDP


END main
