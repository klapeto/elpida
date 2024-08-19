.global atanl
atanl:
	fldt 4(%esp)
	fld1
	fpatan
	ret
