#define NEW

.text
.global strchr
strchr:
	mov	%esi,%edx
	mov	0x4(%esp,1),%esi
	mov	0x8(%esp,1),%cl
	cld
.L1:
	lods	(%esi),%eax
	cmp	%al,%cl
	jz .Lafound
	test	%al,%al
	jz .Lnotfound

	cmp	%ah,%cl
	jz .Lbfound
	test	%ah,%ah
	jz .Lnotfound

	shr	$16,%eax
	cmp	%al,%cl
	jz .Lcfound
	test	%al,%al
	jz .Lnotfound

	cmp	%ah,%cl
	jz .Ldfound
	test	%ah,%ah

	jnz .L1
.Lnotfound:
	sub	%eax,%eax
	mov	%edx,%esi
	ret
.Lafound:
	dec	%esi
.Lbfound:
	dec	%esi
.Lcfound:
	dec	%esi
.Ldfound:
	mov	%esi,%eax
	mov	%edx,%esi
	dec	%eax
	ret
