TITLE Negative Number Fun     (Project03.asm)

; Author: Gaetan Ingrassia
; Last Modified: 2019-02-05
; OSU email address: ingrassg@oregonstate.edu
; Course number/section: CS271-400
; Project Number: #03                Due Date: 2019-02-10
; Description: Prompts the user to enter negative numbers in the range [-100, -1]. Prompting will stop when
;	the user enters a number out of range. Then the number of terms entered, the sum, and the average of the
;	negative numbers will be calculated and displayed.  

INCLUDE Irvine32.inc

LOWER_BOUND			EQU				-100

.data

; (insert variable definitions here)

intro				BYTE			"	   Negative Number Fun   by Gaetan Ingrassia",0
extraCred1			BYTE			"**EC: Numbered lines during user input",0
extraCred2			BYTE			"**EC: Floating-point average rounded to .001",0
namePrompt			BYTE			"Now that you know my name, what is yours? ",0
userName			BYTE			50 DUP(0)	; String to be entered for the user
userGreet1			BYTE			"Hello, ",0
userGreet2			BYTE			"! I hope you are well today!",0
instruct1			BYTE			"Please enter integers in the range [-100, -1] and I will show you",0
instruct2			BYTE			"the total number of negative numbers you entered, the sum of these",0
instruct3			BYTE			"numbers, and the average. Enter a non-negative number to stop.",0
instruct4			BYTE			"Number 1: ",0
instruct5			BYTE			"Number ",0
instruct6			BYTE			": ",0
largeNeg			BYTE			"This negative number is not in range and does not count.",0
noNums				BYTE			"No valid numbers? That's okay, maybe next time!",0
num					DWORD			?
termsDisplay		BYTE			"The number of negative numbers entered: ",0
numTerms			DWORD			0
sumDisplay			BYTE			"The sum of the negative numbers entered: ",0
sum					DWORD			0			; Starting point for the sum of the numbers
avgDisplay			BYTE			"The rounded integer average of the negative numbers entered: ",0
avg					DWORD			0			; Rounded integer average of the entered numbers
avgFloatDisplay		BYTE			"The floating-point average of the negative numbers entered: ",0
thousandth			DWORD			?			; Last digit of the floating point average
decimal				BYTE			".",0		; Decimal point in the average
rMult				DWORD			10			; To multiply the remainder for long division
outro1				BYTE			"GoodBye, ",0
outro2				BYTE			"! Peace and Love!",0

.code
main PROC

; Introduction
	mov		edx, OFFSET intro
	call	WriteString
	call	CrLf
	call	CrLf

	; Extra credit messages
	mov		edx, OFFSET extraCred1
	call	WriteString
	call	CrLf
	mov		edx, OFFSET extraCred2
	call	WriteString
	call	CrLf
	call	CrLf

; Get user data 
	; Get name
	mov		edx, OFFSET namePrompt
	call	WriteString
	mov		edx, OFFSET userName
	mov		ecx, 49
	call	ReadString
	call	CrLf

	; Greet user
	mov		edx, OFFSET userGreet1
	call	WriteString
	mov		edx, OFFSET userName
	call	WriteString
	mov		edx, OFFSET userGreet2
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
	mov		edx, OFFSET instruct3
	call	WriteString
	call	CrLf
	call	CrLf

	; Get first number
	mov		edx, OFFSET instruct4
	call	WriteString
	call	ReadInt
	mov		num, eax
	inc		numTerms

	; Check for valid initial input
	cmp		num, 0
	jns		noNumbers
	mov		eax, LOWER_BOUND
	cmp		num, eax
	jl		notTerminal

	; Continuously check for valid input
inputLoop:
	cmp		num, 0
	jns		loopEnd
	mov		eax, LOWER_BOUND
	cmp		num, eax
	jl		notTerminal

	; Calculate sum and increment number of terms
	mov		eax, num
	add		sum, eax
	inc		numTerms

getNum:
	mov		edx, OFFSET instruct5
	call	WriteString
	mov		eax, numTerms
	call	WriteDec
	mov		edx, OFFSET instruct6
	call	WriteString
	call	ReadInt
	mov		num, eax
	jmp		inputLoop
loopEnd:

	; If terminated with no valid input, jump to special message
	dec		numTerms
	cmp		numTerms, 0
	je		noNumbers

	; If the number is negative and out of range, tell the user
	jmp		notTerminalEnd
notTerminal:
	mov		edx, OFFSET largeNeg
	call	WriteString
	call	CrLf
	jmp		getNum
notTerminalEnd:

; Print data
	call	CrLf
	mov		edx, OFFSET termsDisplay
	call	WriteString
	mov		eax, numTerms
	call	WriteDec
	call	CrLf
	mov		edx, OFFSET sumDisplay
	call	WriteString
	mov		eax, sum
	call	WriteInt
	call	CrLf

	; Calculate rounded integer average
	mov		edx, OFFSET avgDisplay
	call	WriteString
	mov		eax, sum
	cdq
	idiv	numTerms
	mov		avg, eax
	mov		eax, edx
	imul	eax, -10
	mov		edx, 0
	div		numTerms
	cmp		eax, 5
	jle		roundInt
	dec		avg
roundInt:
	mov		eax, avg
	call	WriteInt
	call	CrLf
	
	; Calculate floating-point average
	mov		edx, OFFSET avgFloatDisplay
	call	WriteString
	mov		eax, sum
	cdq
	idiv	numTerms
	call	WriteInt
	mov		eax, edx
	mov		edx, OFFSET decimal
	call	WriteString
	imul	eax, -1
	mov		ecx, 2
decLoop:
	mul		rMult
	div		numTerms
	call	WriteDec
	mov		eax, edx
	loop	decLoop

	; Round the last term
	mul		rMult
	div		numTerms
	mov		thousandth, eax
	mov		eax, edx
	mul		rMult
	div		numTerms
	cmp		eax, 5
	jl		roundDown
	inc		thousandth
roundDown:
	mov		eax, thousandth
	call	WriteDec
	call	CrLf

; Print special message for no entered numbers in range
	jmp		noNumbersEnd
NoNumbers:
	call	CrLf
	mov		edx, OFFSET noNums
	call	WriteString
	call	CrLf
noNumbersEnd:

; farewell
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
