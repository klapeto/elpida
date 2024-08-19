.global log
log:
	fldln2
	fldl 4(%esp)
	fyl2x
	fstpl 4(%esp)
	fldl 4(%esp)
	ret
