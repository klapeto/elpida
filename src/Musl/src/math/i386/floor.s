.global floorf
floorf:
	flds 4(%esp)
	jmp 1f

.global floorl
floorl:
	fldt 4(%esp)
	jmp 1f

.global floor
floor:
	fldl 4(%esp)
1:	mov $0x7,%al
1:	fstcw 4(%esp)
	mov 5(%esp),%ah
	mov %al,5(%esp)
	fldcw 4(%esp)
	frndint
	mov %ah,5(%esp)
	fldcw 4(%esp)
	ret

.global ceil
ceil:
	fldl 4(%esp)
	mov $0xb,%al
	jmp 1b

.global ceilf
ceilf:
	flds 4(%esp)
	mov $0xb,%al
	jmp 1b

.global ceill
ceill:
	fldt 4(%esp)
	mov $0xb,%al
	jmp 1b

.global trunc
trunc:
	fldl 4(%esp)
	mov $0xf,%al
	jmp 1b

.global truncf
truncf:
	flds 4(%esp)
	mov $0xf,%al
	jmp 1b

.global truncl
truncl:
	fldt 4(%esp)
	mov $0xf,%al
	jmp 1b
