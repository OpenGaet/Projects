TITLE Random Numbers     (Project05.asm)

; Author: Gaetan Ingrassia
; Last Modified: 2019-03-05
; OSU email address: ingrassg@oregonstate.edu
; Course number/section: CS271-400
; Project Number: #05                Due Date: 2019-03-05 (2 grace days)
; Description: This program generates an array of random numbers with total number
;	of elements being specified by the user. It then sorts the array in descending
;	order, and displays the unsorted array, the sorted array, and the median.

INCLUDE Irvine32.inc

MIN					EQU					10
MAX					EQU					200
LO					EQU					100
HI					EQU					999

.data

intro				BYTE				"	  Random Number Fun   by Gaetan Ingrassia",0
extraCred1			BYTE				"**EC: The recursive function quickSort is used.",0
instruct1			BYTE				"This program generates random numbers in the range [100, 999],",0
instruct2			BYTE				"displays these numbers as an unsorted list, sorts this list, finds",0 
instruct3			BYTE				"the median value, and displays the sorted list in descending order.",0
prompt				BYTE				"Enter the number of random numbers to display [10, 200]: ",0
request				DWORD				?			; Number of random numbers to be added to the array
invalidNum			BYTE				"That number is out of range, try again.",0
titleUnsorted		BYTE				"The unsorted random numbers: ",0
titleMedian			BYTE				"The median is: ",0
titleSorted			BYTE				"The sorted list: ",0
spacing				BYTE				"	",0
array				DWORD				MAX DUP(?)
pivot				DWORD				?


.code
main PROC

	call	Randomize

	push	OFFSET extraCred1
	push	OFFSET intro
	push	OFFSET instruct1
	push	OFFSET instruct2
	push	OFFSET instruct3
	call	introduction

	push	OFFSET prompt
	push	OFFSET request
	push	OFFSET invalidNum
	call	getData

	push	OFFSET array
	push	request
	call	fillArray

	push	OFFSET spacing
	push	OFFSET titleUnsorted
	push	OFFSET array
	push	request
	call	displayList

	push	OFFSET pivot
	push	OFFSET array
	push	request
	call	sortList

	push	OFFSET titleMedian
	push	OFFSET array
	push	request
	call	displayMedian

	push	OFFSET spacing
	push	OFFSET titleSorted
	push	OFFSET array
	push	request
	call	displayList

	exit	; exit to operating system
main ENDP


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

	push	ebp				
	mov		ebp, esp

	; Greeting 
	mov		edx, [ebp+20]
	call	WriteString
	call	CrLf
	call	CrLf
	; Extra Credit
	mov		edx, [ebp+24]
	call	WriteString
	call	CrLf
	call	CrLf
	; Instructions
	mov		edx, [ebp+16]
	call	WriteString
	call	CrLf
	mov		edx, [ebp+12]
	call	WriteString
	call	CrLf
	mov		edx, [ebp+8]
	call	WriteString
	call	CrLf
	call	CrLf

	pop		ebp
	ret		20
introduction		ENDP


;---------------------------------------------------------------------------------
; getData
;
; Continually prompts user for number in range [10, 200] using a validation loop
;
; Receives:		The addresses of prompt, request, and invalidNum
; Returns:		The number of elements is stored in request
; Registers:	EAX, EBX, EDX, EBP
;---------------------------------------------------------------------------------
getData			PROC

	push	ebp
	mov		ebp, esp
		; Prompt user, validate number, repeat if necessary
inputLoop:
	mov		edx, [ebp+16]		;Prompt
	call	WriteString
	call	ReadInt
	push	eax
	push	[ebp+12]			;Address of request
	call	validate
	cmp		ebx, 1
	je		inputLoopEnd
	mov		edx, [ebp+8]		;invalidNum
	call	WriteString
	call	CrLf
	jmp		inputLoop
inputLoopEnd:
	call	CrLf

	pop		ebp
	ret		12
getData			ENDP


;---------------------------------------------------------------------------------
; validate
;
; Sub-procedure of getData to validate user input is within range
;
; Receives:		The address of request and the value of the user input
; Returns:		Stores user input in request if valid, EBX = 1 if valid, or 
;				BX = 0 if invalid number		
; Registers:	EAX, EBX, EBP
;---------------------------------------------------------------------------------
validate			PROC

	; Set return to 0, change to 1 if number given is within range
	push	ebp				
	mov		ebp, esp
	mov		eax, [ebp+12]		;User input
	mov		ebx, 0
	cmp		eax, MIN
	jl		invalid
	cmp		eax, MAX
	jg		invalid
	mov		ebx, [ebp+8]		;Address of request
	mov		[ebx], eax
	mov		ebx, 1
invalid:

	pop		ebp
	ret		8
validate			ENDP


;---------------------------------------------------------------------------------
; fillArray
;
; Continually prompts user for number in range [1, 400] using a validation loop
; 	 
; Receives:		The address of the array and the value of request
; Returns:		Fills array with the requested nubmer of random numbers
; Registers:	EAX, ECX, EDI, EBP
;---------------------------------------------------------------------------------
fillArray			PROC

	push	ebp
	mov		ebp, esp
	mov		edi, [ebp+12]		;Address of array
	mov		ecx, [ebp+8]		;Value of request

another:
	;Generate random number
	mov		eax, HI
	sub		eax, LO
	inc		eax
	call	RandomRange
	add		eax, LO
	;Store in array
	mov		[edi], eax
	add		edi, 4
	loop	another

	pop		ebp
	ret		8
fillArray			ENDP


;---------------------------------------------------------------------------------
; sortList
;
; Uses QuickSort to sort the array into descending order.
; 	 
; Receives:		The address of the array, the request value, and the address of the pivot.
; Returns:		The original array sorted into descending order, greatest to least.
; Registers:	EAX, EBX, ECX, EDX, ESI, EDI, EBP
;---------------------------------------------------------------------------------
sortList			PROC
	
	push	ebp
	mov		ebp, esp
	mov		esi, [ebp+12]	;Array address
	mov		eax, [ebp+8]	;Request
	dec		eax

	;Call quickSort
	push	[ebp+16]	;Pivot address
	push	esi			;Array address
	push	0			;Low
	push	eax			;High
	call	quickSort

	pop		ebp
	ret		12
sortList			ENDP


;---------------------------------------------------------------------------------
; quickSort
;
; This is a recursive function to sort the array into descending order, by swapping
;	values against a pivot.
; 	 
; Receives:		The pivot address, the array address, the start index value, and the 
;				end index value.
; Returns:		The original array sorted into descending order, greatest to least.
; Registers:	EAX, EBX, ECX, EDX, ESI, EDI, EBP
;---------------------------------------------------------------------------------
; Param: (Arr, low, high) esi=arr,eax=low,ebx=high
quickSort			PROC

	push	ebp
	mov		ebp, esp
	
	mov		ecx, [ebp+12]	;Low value
	mov		ebx, [ebp+8]	;High value

	;Base Case
	cmp		ecx, ebx		;If low >= high, return
	jge		endSort
	;Call Partition
	push	[ebp+20]		;address of pivot var
	push	[ebp+16]		;Array address
	push	[ebp+12]		;Low value
	push	[ebp+8]			;High value
	call	partition
	mov		esi, [ebp+20]	;Move new pivot into eax
	mov		eax, [esi]
	;Call quickSort on lower half
	push	[ebp+20]		;address of pivot var
	push	[ebp+16]		;Array address
	push	[ebp+12]		;Low index
	cmp		eax, 0
	je		ifZero
	dec		eax
	ifZero:
	push	eax				;Pivot Index - 1
	call	quickSort
	;Call quickSort on upper half
	push	[ebp+20]		;address of pivot var
	push	[ebp+16]		;Array address
	mov		esi, [ebp+20]	;Move pivot into eax
	mov		eax, [esi]
	inc		eax
	push	eax				;Pivot Index + 1
	push	[ebp+8]			;High index
	call	quickSort
endSort:

	pop		ebp
	ret		16
quickSort			ENDP


;---------------------------------------------------------------------------------
; partition
;
; This function sets the pivot to the first element and swaps the rest of the elements,
;	moving them to the right of the pivot if they are less than, and to the left if they
;	are greater than.
; 	 
; Receives:		The pivot address, the array address, the start index value, and the 
;				end index value.
; Returns:		A partially sorted array, with lesser value to the right, and greater
;				values to the left.
; Registers:	EAX, EBX, ECX, EDX, ESI, EDI, EBP
;---------------------------------------------------------------------------------
partition			PROC
	
	push	ebp
	mov		ebp, esp
	push	ebx				;Preserve registers
	push	ecx

	;Find and set pivot position
	mov		esi, [ebp+16]	;Array address
	mov		eax, [ebp+12]	;Starting Index
	imul	eax, 4
	add		esi, eax
	;Set the pivot va1ue
	mov		ebx, [esi]		;ebx is the pivot value
	mov		eax, esi

	;Set ecx to the starting position of to the right of the pivot
	mov		ecx, [ebp+12]
	inc		ecx
	add		esi, 4
	;Loop while the end index has not been reached 
forLoopBegin:
	;Do not swap if the current value is less than or equal to the pivot 
	cmp		[esi], ebx 
	jle		noSwap
	;Call swap for array[pivotPosition+1] and arr[current]
	add		eax, 4
	push	eax
	sub		eax, 4
	push	esi
	call	swapElem
	;Call swap for array[pivotPosition] and array[pivotPosition + 1])
	push	eax
	add		eax, 4
	push	eax
	sub		eax, 4
	call	swapElem
	;Increment the pivot position
	add		eax, 4
noSwap:
	;Increment the current position
	add		esi, 4
	inc		ecx
	cmp		ecx, [ebp+8]
	jg		forLoopEnd
	jmp		forLoopBegin
forLoopEnd:
	;Restore eax from a memory address to an integer index
	sub		eax, [ebp+16]
	mov		ecx, 4
	xor		edx, edx
	div		ecx
	;Set pivot to new pivot position; 
	mov		edi, [ebp+20]
	mov		[edi], eax

	pop		ecx				;Restore registers
	pop		ebx
	pop		ebp
	ret		16
partition			ENDP


;---------------------------------------------------------------------------------
; swapElem
;
; Swaps the values of the given array elements.
; 
; Receives:		The addresses of two array elements to be swapped
; Returns:		The original array with the elements swapped
; Registers:	EBX, ECX, EDX, EDI, EBP
;---------------------------------------------------------------------------------
swapElem			PROC
	
	push	ebp
	mov		ebp, esp
	pushad					;Preserve Registers

	mov		edi, [ebp+12]	;First element
	mov		ebx, [edi]
	mov		ecx, [ebp+8]	;Second Element
	mov		edx, [ecx]
	;Swap the elements
	mov		[edi], edx
	mov		edi, [ebp+8]
	mov		[edi], ebx

	popad					;Restore Registers
	pop		ebp
	ret		8
swapElem			ENDP


;---------------------------------------------------------------------------------
; displayMedian
;
; Caculates and displays the median of the sorted array.
; 
; Receives:		The addresses of titleMedian, the array, and the value of request.
; Returns:		Displays the median in output
; Registers:	EAX, ECX, EDX, ESI, EBP
;---------------------------------------------------------------------------------
displayMedian			PROC

	push	ebp
	mov		ebp, esp
	mov		esi, [ebp+12]		
	mov		eax, [ebp+8]

	;If request is odd, set median to middle element
	mov		ecx, 2
	xor		edx, edx
	div		ecx
	imul	eax, 4
	add		esi, eax		
	mov		eax, [esi]
	;If request is even, find the average of the two middle elements
	cmp		edx, 1
	je		odd
	add		eax, [esi-4]
	div		ecx
	cmp		edx, 1
	jne		odd
	inc		eax
odd:
	;Display the resulting median
	call	CrLf
	mov		edx, [ebp+16]
	call	WriteString
	call	WriteDec
	call	CrLf
	call	CrLf

	pop		ebp
	ret		12
displayMedian			ENDP


;---------------------------------------------------------------------------------
; displayList
;
; Displays the elements of the given array in rows with 10 columns.
; 
; Receives:		The addresses of the given title, spacing, and the array, as well 
;				as the value of request.
; Returns:		Displays the given array to output
; Registers:	EAX, EBX, ECX, EDX, ESI, EBP
;---------------------------------------------------------------------------------
displayList			PROC

	push	ebp
	mov		ebp, esp
	mov		edx, [ebp+16]	;Title
	mov		esi, [ebp+12]	;Array address
	mov		ecx, [ebp+8]	;Request value

	;Print given title (sorted/unsorted)
	call	WriteString
	call	CrLf

	;Loop through array, displaying each element
	mov		ecx, [ebp+8]
	mov		ebx, 1
displayLoop:
	mov		eax, [esi]
	call	WriteDec
	add		esi, 4
	mov		edx, [ebp+20]	;Sapcing address
	call	WriteString
	; Check column number per row
	inc		ebx
	cmp		ebx, 10			;Number of elements per row
	jle		sameLine
	mov		ebx, 1
	call	CrLf
sameLine:
	loop	displayLoop
	call	CrLf

	pop		ebp
	ret		16
displayList			ENDP

END main
