TITLE Maths     (Project01.asm)

; Author: Gaetan Ingrassia
; Last Modified: 2019-01-19
; OSU email address: ingrassg@oregonstate.edu
; Course number/section: CS271-400
; Project Number: #01                Due Date: 2019-01-20
; Description: This program will introduce the programmer, ask the user to enter
;	two numbers, and calculate and display the sum, difference, product, integer
;	quotient, and remainder of the two numbers.

INCLUDE Irvine32.inc

; (insert constant definitions here)

.data

numOne		DWORD		?			; first number
numTwo		DWORD		?			; second number
sum			DWORD		?			; sum of the numbers entered
diff		DWORD		?			; difference of the numbers
product		DWORD		?			; product of the numbers
quotient	DWORD		?			; integer quotient of the numbers
remainder	DWORD		?			; remainder of the numbers
intro		BYTE		"		Math Fun   by Gaetan Ingrassia",0
extraCred1	BYTE		"**EC1: Program verifies second number is less than first.",0
extraCred2	BYTE		"**EC2: Program repeats until the user chooses to quit.",0
spacing		BYTE		" ",0
instruct1	BYTE		"Please enter two numbers. Once entered, sit back and relax while",0
instruct2	BYTE		"I, your humble servant, calculate the maths. I will show you",0
instruct3	BYTE		"the sum, difference, product, integer quotient and remainder.",0
prompt1		BYTE		"Number 1: ",0
prompt2		BYTE		"Number 2: ",0
equals		BYTE		" = ",0
sumSign		BYTE		" + ",0
diffSign	BYTE		" - ",0
prodSign	BYTE		" * ",0
quoSign		BYTE		" / ",0
remSign		BYTE		" remainder ",0
outro		BYTE		"Peace and Love. Goodbye, Friend!",0
condition	BYTE		"The second number must be less than the first!",0
toRepeat	BYTE		"Perform another calculation? (0 = no, 1 = yes): ",0
loopCheck	DWORD		?			; Checks if the user wants to run the program again

.code
main PROC

; Display name and title
	mov		edx, OFFSET intro
	call	WriteString
	call	CrLf
	mov		edx, OFFSET spacing
	call	WriteString
	call	CrLf
	mov		edx, OFFSET extraCred1
	call	WriteString
	call	CrLf
	mov		edx, OFFSET extraCred2
	call	WriteString
	call	CrLf
	mov		edx, OFFSET spacing
	call	WriteString
	call	CrLf

; Display instructions
	mov		edx, OFFSET instruct1
	call	WriteString
	call	CrLf
	mov		edx, OFFSET instruct2
	call	WriteString
	call	CrLf
	mov		edx, OFFSET instruct3
	call	WriteString
	call	CrLf
	mov		edx, OFFSET spacing
	call	WriteString
	call	CrLf

loop1:									; Start of loop
; Prompt user for two numbers
	mov		edx, OFFSET prompt1			; Prompt for first number and store in numOne
	call	WriteString
	call	ReadInt
	mov		numOne, eax
	mov		edx, OFFSET prompt2			; Prompt for second number and store in numTwo
	call	WriteString
	call	ReadInt
	mov		numTwo, eax
	call	CrLf

	mov		eax, numOne					; Make sure the second number entered is less than the first
	mov		ebx, numTwo
	cmp		eax, ebx
	jle		less						; If first number is not greater, jump to error message

; Calculate the sum, difference, product, integer quotient and remainder
	mov		eax, numOne					; Sum Operations
	mov		ebx, numTwo
	add		eax, ebx
	mov		sum, eax

	mov		eax, numOne					; Difference Operations
	mov		ebx, numTwo
	sub		eax, ebx
	mov		diff, eax

	mov		eax, numOne					; Product Operations
	mov		ebx, numTwo
	mul		ebx
	mov		product, eax

	mov		eax, numOne					; Quotient Operations
	mov		ebx, numTwo
	div		ebx
	mov		quotient, eax
	mov		remainder, edx

; Display results
	mov		eax, numOne					; Sum Display
	call	WriteDec
	mov		edx, OFFSET sumSign
	call	WriteString
	mov		eax, numTwo
	call	WriteDec
	mov		edx, OFFSET equals
	call	WriteString
	mov		eax, sum
	call	WriteDec
	call	CrLf

	mov		eax, numOne					; Difference Display
	call	WriteDec
	mov		edx, OFFSET diffSign
	call	WriteString
	mov		eax, numTwo
	call	WriteDec
	mov		edx, OFFSET equals
	call	WriteString
	mov		eax, diff
	call	WriteDec
	call	CrLf

	mov		eax, numOne					; Product Display
	call	WriteDec
	mov		edx, OFFSET prodSign
	call	WriteString
	mov		eax, numTwo
	call	WriteDec
	mov		edx, OFFSET equals
	call	WriteString
	mov		eax, product
	call	WriteDec
	call	CrLf

	mov		eax, numOne					; Quotient Display
	call	WriteDec
	mov		edx, OFFSET quoSign
	call	WriteString
	mov		eax, numTwo
	call	WriteDec
	mov		edx, OFFSET equals
	call	WriteString
	mov		eax, quotient
	call	WriteDec
	mov		edx, OFFSET remSign
	call	WriteString
	mov		eax, remainder
	call	WriteDec
	call	CrLf

	mov		edx, OFFSET spacing			; Add spacing
	call	WriteString
	call	CrLf

	jmp		done						; Jump to the loop check to avoid error message

; Error message if the second number is greater than the first
less:	mov		edx, OFFSET condition
		call	WriteString
		call	CrLf
		
; Ask the user if they would like to run the program again
done:	mov		edx, OFFSET toRepeat
		call	WriteString
		call	ReadInt
		mov		loopCheck, eax

		cmp		loopCheck, 1
		je		loop1


; Display a terminating message
		mov		edx, OFFSET spacing
		call	WriteString
		call	CrLf
		mov		edx, OFFSET outro
		call	WriteString
		call	CrLf

		exit							; exit to operating system

main ENDP

; (insert additional procedures here)

END main