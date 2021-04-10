define ti? ti
namespace ti?
pSpiRange   := 0D000h
mpSpiRange  := 0F80000h
spiValid    := 8
pSpiValid   := pSpiRange + spiValid
mpSpiValid  := mpSpiRange + spiValid
spiStatus   := 12
pSpiStatus  := pSpiRange + spiStatus
mpSpiStatus := mpSpiRange + spiStatus
spiData     := 24
pSpiData    := pSpiRange + spiData
mpSpiData   := mpSpiRange + spiData
end namespace

	public	_spi_write
_spi_write:
	pop	hl,de
	push	de,hl
	ld	hl,ti.mpSpiValid or 1 shl 8
	ld	(hl),h
	ld	a,(de)
	ld	c,a
virtual
	or	a,0
 load .or_a: byte from $$
end virtual
	db	.or_a
.loop:
	scf
	inc	de
	ld	a,(de)
	ld	l,ti.spiData
	ld	b,3
.shift:
	rla
	rla
	rla
	ld	(hl),a
	djnz	.shift
	ld	l,ti.spiStatus+1
.wait:
	bit	15-8,(hl)
	jq	nz,.wait
.enter:
	dec	c
	jq	nz,.loop
	ret
