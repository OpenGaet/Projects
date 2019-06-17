TITLE Sum and Average     (Project6A.asm)

; Author: Gaetan Ingrassia
; Last Modified: 2019-03-17
; OSU email address: ingrassg@gmail.com
; Course number/section: CS271-400
; Project Number: #06A                Due Date: 2019-03-17
; Description: This program receives 10 integers from the user and displays
;	the numbers, their sum, and their average.

INCLUDE Irvine32.inc

NUM_TERMS			EQU					10

.data

intro				BYTE				"			Sum and Average   by Gaetan Ingrassia",0
instruct1			BYTE				"Please provide 10 unsigned decimal integers. Every number must fit in a 32 bit register.",0
instruct2			BYTE				"Once you have entered the numbers, I will display them, along with their sum and average.",0
prompt				BYTE				"Enter an unsigned number please: ",0
input				BYTE				15 dUP(0)			; Number of random numbers to be added to the array
invalidNum			BYTE				"ERROR: That was either not an unsigned number or it was too large.",0
numArray			DWORD				NUM_TERMS DUP(?)	; Only 10 input numbers allowed
sum					DWORD				0
numMessage			BYTE				"You entered the following numbers:",0
spacing				BYTE				", ",0
sumMessage			BYTE				"The sum of these numbers is: ",0
avgMessage			BYTE				"The average is: ",0
outMessage			BYTE				"GoodBye!",0

.code
main PROC

	push			OFFSET intro
	push			OFFSET instruct1
	push			OFFSET instruct2
	call			introduction
		
	push			OFFSET numArray
	push			OFFSET prompt
	push			OFFSET input
	push			OFFSET invalidNum
	call			readVal
	
	push			OFFSET numMessage
	push			OFFSET input
	push			OFFSET spacing
	push			OFFSET numArray
	call			displayArray

	push			OFFSET sum
	push			OFFSET sumMessage
	push			OFFSET input
	push			OFFSET numArray
	call			displaySum

	push			OFFSET sum
	push			OFFSET avgMessage
	push			OFFSET input
	call			displayAvg
	
	push			OFFSET outMessage
	call			outro

	exit	; exit to operating system
main ENDP


;---------------------------------------------------------------------------------------------
; Macro to print string at the address of toDisplay. Registers preserved.
; Parameters:	[string address]
;---------------------------------------------------------------------------------------------
displayString MACRO toDisplay

	push			edx
	mov				edx, toDisplay
	call			WriteString
	pop				edx

ENDM


;---------------------------------------------------------------------------------
; introduction
;
; Introduces the program, its rules, and the programmer to the user
; 
; Receives:		The addresses of extraCred1, intro, and instruct"1, 2, 3"
; Returns:		NA
; Registers:	EDX, EBP
;---------------------------------------------------------------------------------
introduction		PROC

	push			ebp				
	mov				ebp, esp
	pushad

	; Greeting 
	displayString	[ebp+16]
	call			CrLf
	call			CrLf
	; Instructions
	displayString	[ebp+12]
	call			CrLf
	displayString	[ebp+8]
	call			CrLf
	call			CrLf

	popad
	pop				ebp
	ret				12
introduction		ENDP


;---------------------------------------------------------------------------------------------
; Macro to store user entered string in the address of userIn. Registers preserved.
; Parameters:	[string address, string address]
;---------------------------------------------------------------------------------------------
getString	MACRO	instruct, userIn

	push			edx
	push			ecx
	displayString	instruct
	mov				edx, userIn
	mov				ecx, 15				; Max allowed characters in string input
	call			ReadString
	pop				ecx
	pop				edx

ENDM


;----------------------------------------------------------------------------------------------
; readVal
;
; Gets a string of digits from the user, validates input, and converts it to a 32-bit integer
; 
; Receives:		The addresses of numArray, prompt, input, and invalidNum
; Returns:		An array of 10, 32-bit integers stored in numArray
; Registers:	EAX, EBX, ECX, EDX, EDI, ESI, EBP
;----------------------------------------------------------------------------------------------
readVal				PROC

	push			ebp
	mov				ebp, esp
	pushad
	mov				edi, [ebp+20]		; Number array
	
; Loop while numArray is not filled
	mov				ecx, NUM_TERMS		; Number of terms allowed in array sets counter
inputLoop:
	; Clear input string on each iteration
	push			ecx
	push			edi
	mov				al, 0
	mov				edi, [ebp+12]
	cld
	rep				stosb
	pop				edi
	pop				ecx
	; Get string with macro getString
	xor				ebx, ebx
	getString		[ebp+16], [ebp+12]	; [@prompt, @input]

	; Validate user input
	mov				edx, 1				; Maintain character count in EDX
	mov				esi, [ebp+12]		; @input
stringLoop:
	xor				eax, eax
	lodsb
	
	cmp				eax, 0				; Check for terminating value
	je				endString
	cmp				edx, 10				; Check digit number within 32-bit range
	jg				notValid
	cmp				eax, 48				; Decimal value of '0'
	jb				notValid
	cmp				eax, 57				; Decimal value of '9'
	ja				notValid

	; Convert string to 32-bit integer
convert:
	push			edx
	push			ecx
	xor				edx, edx
	sub				eax, '0'
	push			eax
	clc
	mov				eax, ebx
	mov				ecx, 10
	mul				ecx
	jc				tooLarge1			; Check if conversion sets carry flag
	mov				ebx, eax
	clc
	pop				eax
	add				ebx, eax
	jc				tooLarge2			; Check if conversion sets carry flag
	pop				ecx
	pop				edx
	inc				edx
	jmp				stringLoop
endString:
	cmp				edx, 1				; Check for input no input
	je				notValid

	; Move converted number to next empty element of the array
	mov				[edi], ebx
	add				edi, 4
	dec				ecx
	cmp				ecx, 0
	je				inputLoopEnd
	jmp				inputLoop

	; If input is too large, negative, or not a number, give user error
tooLarge1:								; Maintain stack
	pop				eax
tooLarge2:								; Maintain stack
	pop				ecx
	pop				edx
notValid:
	push			edx
	displayString	[ebp+8]				; @invalidNum
	call			CrLf
	pop				edx
	jmp				inputLoop

inputLoopEnd:

	popad
	pop				ebp
	ret				16
readVal				ENDP


;---------------------------------------------------------------------------------
; displayArray
;
; Converts the values of numArray to individual strings and prints them
; 
; Receives:		The addresses of, numMessage, input, spacing, numArray	
; Returns:		NA
; Registers:	EAX, EBX, ECX, EDX, EDI, ESI, EBP. All preserved
;---------------------------------------------------------------------------------
displayArray		PROC

	push			ebp
	mov				ebp, esp
	pushad

	call			CrLf
	displayString	[ebp+20]			;numMessage
	call			CrLf
	mov				esi, [ebp+8]		;numArray

; Loop through array, converting and printing each number as a string
	mov				ecx, NUM_TERMS
arrayLoop:
	push			ecx
	mov				eax, [esi]
	mov				ecx, 0
	mov				ebx, 10
	; Calculate length of integer
countLoop:
	xor				edx, edx
	div				ebx
	inc				ecx
	cmp				eax, 0
	jne				countLoop
	; Call writeVal
	push			[esi]				;integer
	push			[ebp+16]			;input
	push			ecx					;Number length
	call			writeVal
	pop				ecx
	; Write spacing
	cmp				ecx, 1				; Check if last number, don't add comma if so
	je				noComma
	displayString	[ebp+12]
noComma:
	add		esi, 4
	loop	arrayLoop
	call	CrLf

	popad
	pop				ebp
	ret				16
displayArray		ENDP


;---------------------------------------------------------------------------------
; displaySum
;
; Calculates the sum of the array, converts it to a string, and prints it
; 
; Receives:		The addresses of sum, sumMessage, input, and numArray
; Returns:		Sum of array stored in sum, and sum printed
; Registers:	EAX, EBX, ECX, EDX, EDI, ESI, EBP. All preserved
;---------------------------------------------------------------------------------
displaySum			PROC

	push			ebp
	mov				ebp, esp
	pushad

	mov				esi, [ebp+8]		; @number Array
	mov				edi, [ebp+20]		; @sum

	displayString	[ebp+16]			; @sum Message

	; Calculate Sum
	mov				ecx, NUM_TERMS
	xor				eax, eax
sumLoop:								
	add				eax, [esi]
	add				esi, 4
	loop			sumLoop
	mov				[edi], eax

	; Calculate length of sum
	mov				ecx, 0
	mov				ebx, 10
sumLength:
	xor				edx, edx
	div				ebx
	inc				ecx
	cmp				eax, 0
	jne				sumLength

	; Call writeVal
	push			[edi]				; integer
	push			[ebp+12]			; @input
	push			ecx					; integer length
	call			writeVal
	call			CrLf

	popad
	pop				ebp
	ret				16
displaySum			ENDP


;---------------------------------------------------------------------------------
; displayAvg
;
; Calculates the average of the array, converts it to a string, and prints it
; 
; Receives:		The addresses of sum, avgMessage, and input
; Returns:		Printed average
; Registers:	EAX, EBX, ECX, EDX, EDI, ESI, EBP. All preserved
;---------------------------------------------------------------------------------
displayAvg			PROC

	push			ebp
	mov				ebp, esp
	pushad

	mov				esi, [ebp+16]		; @sum

	; Calculate Average
	displayString	[ebp+12]			; @avgMessage
	mov				ebx, NUM_TERMS
	mov				eax, [esi]
	xor				edx, edx
	div				ebx
	push			eax

	; Calculate length of average
	mov				ecx, 0
	mov				ebx, 10
avgLength:
	xor				edx, edx
	div				ebx
	inc				ecx
	cmp				eax, 0
	jne				avgLength
	pop				eax

	; Call writeVal
	push			eax					; average
	push			[ebp+8]				; @input
	push			ecx					; integer length
	call			writeVal
	call			CrLf

	popad
	pop				ebp
	ret				12
displayAvg			ENDP


;----------------------------------------------------------------------------------------------
; writeVal
;
; Converts the integer value passed in to a string and prints it
; 
; Receives:		The integer to be converted, its length, and the address of input
; Returns:		A string version of the integer, stored in input and printed
; Registers:	EAX, EBX, ECX, EDX, EDI, ESI, EBP. All preserved
;----------------------------------------------------------------------------------------------
writeVal			PROC

	push			ebp
	mov				ebp, esp
	pushad

	; Clear output string
	push			ecx
	push			edi
	mov				al, 0
	mov				edi, [ebp+12]		; @input
	mov				ecx, 15
	cld
	rep				stosb
	pop				edi
	pop				ecx

	; Load integer
	mov				eax, [ebp+16]		; integer
	mov				edi, [ebp+12]		; @input
	add				edi, [ebp+8]		; integer lengtth
	dec				edi

	; Convert 32-bit integer to string and store in input
	mov				ecx, [ebp+8]
convertLoop:
	mov				ebx, 10
	xor				edx, edx
	div				ebx
	add				edx, '0'
	push			eax
	mov				eax, edx
	std
	stosb
	pop				eax
	loop			convertLoop

	; Display completed string
endString:
	displayString	[ebp+12]

	popad
	pop				ebp
	ret				12
writeVal			ENDP


;---------------------------------------------------------------------------------
; outro
;
; Bids a tearful adieu to the user
; 
; Receives:		The addresses of outro
; Returns:		A sad farewell output string
; Registers:	EBP. All preserved
;---------------------------------------------------------------------------------
outro				PROC

	push			ebp
	mov				ebp, esp

	call			CrLf
	displayString	[ebp+8]				; @outro
	call			CrLf

	pop				ebp
	ret				8
outro				ENDP


END main

