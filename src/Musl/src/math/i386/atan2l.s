.global atan2l
atan2l:
	fldt 4(%esp)
	fldt 16(%esp)
	fpatan
	ret
