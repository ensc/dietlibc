.text
.global sleep
.type   sleep,@function

sleep:
	movl    4(%esp),%eax            # Argument holen
	pushl   $0                      # 0 ns
	pushl   %eax                    # 'x' µs warten
	movl    %esp,%eax
	pushl   %eax                    # zweimal ein Zeiger auf das Stackobjekt
	pushl   %eax                    # ptr  ptr  sec  nsec  return  arg
	call    nanosleep
	movl    20(%esp),%eax           # 'x' holen
	subl    8(%esp),%eax            # schon abgelaufende Zeit subtrahieren
	addl    $16,%esp
	ret
