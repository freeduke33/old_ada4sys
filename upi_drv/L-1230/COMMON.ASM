	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif
	?debug	V 300h
	?debug	S "common.c"
	?debug	C E921036D2008636F6D6D6F6E2E63
	?debug	C E94019C9E013643A5C62635C696E636C7564655C646F732E68
	?debug	C E94019C9E015643A5C62635C696E636C7564655C5F646566732E68
	?debug	C E94019C9E015643A5C62635C696E636C7564655C636F6E696F2E68
	?debug	C E92A5D6A20076C636172642E68
_TEXT	segment byte public 'CODE'
_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_TEXT	segment byte public 'CODE'
   ;	
   ;	lcRet putCmd(int base,int cmd)
   ;	
	assume	cs:_TEXT
_putCmd	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	 int v=0;
   ;	
	xor	di,di
   ;	
   ;	
   ;	 outport(COMMAND+base,cmd);
   ;	
	mov	dx,si
	add	dx,8
	mov	ax,word ptr [bp+6]
	out	dx,ax
   ;	
   ;	 inport(DATA+base);
   ;	
	mov	dx,si
	in	ax,dx
	jmp	short @1@86
@1@58:
   ;	
   ;	 while(!(inport(CONTROL+base) & 0x02) && v++<10000) cputs("c\b");
   ;	
	mov	ax,offset DGROUP:s@
	push	ax
	call	near ptr _cputs
	pop	cx
@1@86:
	mov	dx,si
	add	dx,12
	in	ax,dx
	test	ax,2
	jne	short @1@142
	mov	ax,di
	inc	di
	cmp	ax,10000
	jl	short @1@58
@1@142:
   ;	
   ;	
   ;	 return lc_NONE;
   ;	
	xor	ax,ax
	jmp	short @1@170
@1@170:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_putCmd	endp
   ;	
   ;	lcRet putData(int base,int val)
   ;	
	assume	cs:_TEXT
_putData	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	 int r,v=0;
   ;	
	xor	di,di
   ;	
   ;	 outport(DATA+base,val);
   ;	
	mov	dx,si
	mov	ax,word ptr [bp+6]
	out	dx,ax
	jmp	short @2@86
@2@58:
   ;	
   ;	 while(!((r=inport(CONTROL+base)) & 0x02) && v++<10000) cputs("w\b");
   ;	
	mov	ax,offset DGROUP:s@+3
	push	ax
	call	near ptr _cputs
	pop	cx
@2@86:
	mov	dx,si
	add	dx,12
	in	ax,dx
	mov	word ptr [bp-2],ax
	test	ax,2
	jne	short @2@142
	mov	ax,di
	inc	di
	cmp	ax,10000
	jl	short @2@58
@2@142:
   ;	
   ;	
   ;	 return lc_NONE;
   ;	
	xor	ax,ax
	jmp	short @2@170
@2@170:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_putData	endp
   ;	
   ;	lcRet getData(int base,int *val)
   ;	
	assume	cs:_TEXT
_getData	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
	mov	si,word ptr [bp+4]
	mov	di,word ptr [bp+6]
   ;	
   ;	{
   ;	 int v=0;
   ;	
	mov	word ptr [bp-2],0
	jmp	short @3@86
@3@58:
   ;	
   ;	
   ;	 while(!(inport(CONTROL+base) & 0x01) && v++<10000) cputs("r\b");
   ;	
	mov	ax,offset DGROUP:s@+6
	push	ax
	call	near ptr _cputs
	pop	cx
@3@86:
	mov	dx,si
	add	dx,12
	in	ax,dx
	test	ax,1
	jne	short @3@142
	mov	ax,word ptr [bp-2]
	inc	word ptr [bp-2]
	cmp	ax,10000
	jl	short @3@58
@3@142:
   ;	
   ;	 *val=inport(DATA+base);
   ;	
	mov	dx,si
	in	ax,dx
	mov	word ptr [di],ax
   ;	
   ;	
   ;	 return lc_NONE;
   ;	
	xor	ax,ax
	jmp	short @3@170
@3@170:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_getData	endp
	?debug	C E9
	?debug	C FA00000000
_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'c'
	db	8
	db	0
	db	'w'
	db	8
	db	0
	db	'r'
	db	8
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	public	_getData
	public	_putData
	public	_putCmd
	extrn	_cputs:near
_s@	equ	s@
	end
