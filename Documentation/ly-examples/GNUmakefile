depth=../..
LOCALSTEPMAKE_TEMPLATES=lilypond ly
include $(depth)/make/stepmake.make

LILYPOND_PREVIEW = $(LILYPOND_BINARY) -dpreview -dresolution=150

OUT_PNG_FILES = $(LY_FILES:%.ly=$(outdir)/%.png)
OUT_PNG_PAGES = $(PNG_PAGES:%=$(outdir)/%.png)

OUT_SMALL_PNG_FILES = $(OUT_PNG_FILES:%.png=%-small.png)

ifeq ($(out),www)
local-WWW-1: $(OUT_PNG_FILES) $(OUT_SMALL_PNG_FILES)
endif

$(outdir)/%.png: %.ly
	$(buildscript-dir)/run-and-check "$(LILYPOND_PREVIEW) -o $(outdir) $<" "$*.preview.log"
	mv $(outdir)/$*.preview.png $@

$(outdir)/%-small.png: $(outdir)/%.png
	pngtopnm $< | pnmscale -w=600 | pnmtopng > $@ 2> /dev/null
