TITLE Fibonacci Numbers     (Project02.asm)

; Author: Gaetan Ingrassia
; Last Modified: 2019-01-27
; OSU email address: ingrassg@oregonsate.edu
; Course number/section: CS271-400
; Project Number: #02                Due Date: 2019-01-27
; Description: This program will display a sequence of Fibonacci numbers with n elements,
;	n being specified by the user and in the range [1, 46].

INCLUDE Irvine32.inc

; (insert constant definitions here)
LOWER_BOUND		EQU				1
UPPER_BOUND		EQU				46

.data

; (insert variable definitions here)

intro			BYTE			"  Fibonacci Fun   by Gaetan Ingrassia",0
extraCred		BYTE			"**EC: Displays numbers in aligned columns.",0
namePrompt		BYTE			"Now that you know my name, what is yours? ",0
userName		BYTE			50 DUP(0)	; String to be entered for the user
userGreet1		BYTE			"Hello, ",0
userGreet2		BYTE			"! I hope you are well today!",0
instruct1		BYTE			"Please enter an integer in the range [1, 46] so I can show you how",0
instruct2		BYTE			"many Fibonacci numbers I know: ",0
numTerms		DWORD			?
valid			BYTE			"Thank you! Look at all the numbers I know!",0
fibSeed			DWORD			1			; First Fibonacci term
spacing			BYTE			"	",0
column			DWORD			1			; Keeps track of current column
row				DWORD			1			; Keeps track of current row
outro1			BYTE			"GoodBye, ",0
outro2			BYTE			"! Peace and Love!",0

.code
main PROC

; Introduction

	mov		edx, OFFSET intro
	call	WriteString
	call	CrLf
	call	CrLf
	mov		edx, OFFSET extraCred
	call	WriteString
	call	CrLf
	call	CrLf

; userInstructions 

	; Get name
	mov		edx, OFFSET namePrompt
	call	WriteString
	mov		edx, OFFSET userName
	mov		ecx, 49
	call	ReadString
	call	CrLf
	mov		edx, OFFSET userGreet1
	call	WriteString
	mov		edx, OFFSET userName
	call	WriteString
	mov		edx, OFFSET userGreet2
	call	WriteString
	call	CrLf
	call	CrLf

	; getUserData
inputLoop:
	mov		edx, OFFSET instruct1
	call	WriteString
	call	CrLf
	mov		edx, OFFSET instruct2
	call	WriteString
	call	ReadInt
	mov		numTerms, eax
	call	CrLf

	; Check for valid input
	mov		eax, LOWER_BOUND
	cmp		numTerms, eax
	jl		inputLoop
	mov		eax, UPPER_BOUND
	cmp		numTerms, eax
	jg		inputLoop
	mov		edx, OFFSET valid
	call	WriteString
	call	CrLf
	call	CrLf

; displayfibs

	; Calculate the Fibonacci Terms
	mov		eax, fibSeed
	mov		ebx, fibSeed
	mov		ecx, numTerms
	call	WriteDec
	cmp		numTerms, 1			; If number of terms is 1, we can skip the loop
	je		endLoop
	dec		ecx	
	dec		column
	mov		edx, OFFSET spacing
	call	WriteString
	call	WriteString
fibLoop:
	inc		column
	cmp		column, 5			; Make sure there are only 5 elements per row
	jne		sameLine
	call	CrLf
	mov		column, 0
	inc		row
sameLine:
	call	WriteDec
	mov		edx, OFFSET spacing
	call	WriteString
	cmp		row, 7				; Add more spacing to smaller numbers to match large numbers
	jg		longNum
	mov		edx, OFFSET spacing
	call	WriteString
longNum:
	;;
	add		eax, fibSeed
	mov		fibSeed, ebx
	mov		ebx, eax
	loop	fibLoop

endLoop:

; farewell

	call	CrLf
	call	CrLf
	mov		edx, OFFSET outro1
	call	WriteString
	mov		edx, OFFSET userName
	call	WriteString
	mov		edx, OFFSET outro2
	call	WriteString
	call	CrLf


	exit	; exit to operating system
main ENDP

; (insert additional procedures here)

END main
