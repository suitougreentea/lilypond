;;;
;;; font.scm -- implement Font stuff
;;;
;;; source file of the GNU LilyPond music typesetter
;;; 
;;; (c) 2000 Jan Nieuwenhuizen <janneke@gnu.org>
;;;

	 
;; Corresponding properties:
;;
;;   font-series font-shape font-family font-name font-point font-size
;;

(define style-to-font-alist
  '(
    (finger . "* * number * * -3")
    (volta . "* * number * * -2")
    (timesig . "* * number * * 0")
    (mmrest . "* * number * * 0")
    (mark . "* * number * * 1")
    (script . "* * roman * * -1")
    (large . "* * roman * * 1")
    (Large . "bold * roman * * 2")
    (dynamic . "bold * dynamic * * 0")
    ))

(define paper20-style-sheet-alist-template
  '(
    (("medium upright music feta 20" . 0) . "feta20")
    (("medium upright music feta 16" . -1) . "feta16")
    (("medium upright music feta 13" . -2) . "feta13")
    (("medium upright music feta 13" . -3) . "feta11")
    (("medium upright music feta 13" . -4) . "feta11")
    (("medium upright music feta 23" . 1) . "feta23")
    (("medium upright music feta 26" . 2) . "feta26")
    (("medium upright braces feta-braces 20" . 0) . "feta-braces20")
    (("bold italic dynamic feta-din 14" . 2) . "feta-din14")
    (("bold italic dynamic feta-din 13" . 1) . "feta-din13")
    (("bold italic dynamic feta-din 12" . 0) . "feta-din12")
    (("bold italic dynamic feta-din 10" . -1) . "feta-din10")
    (("bold italic dynamic feta-din 8" . -2) . "feta-din8")
    (("bold italic dynamic feta-din 7" . -3) . "feta-din7")
    (("bold italic dynamic feta-din 6" . -4) . "feta-din6")
    (("bold italic dynamic feta-din 5" . -5) . "feta-din5")
    (("bold italic dynamic feta-din 4" . -6) . "feta-din4")
    ;; Hmm
    (("medium upright number feta-nummer 13" . 3) . "feta-nummer14")
    (("medium upright number feta-nummer 13" . 2) . "feta-nummer13")
    (("medium upright number feta-nummer 12" . 1) . "feta-nummer12")
    (("medium upright number feta-nummer 10" . 0) . "feta-nummer10")
    (("medium upright number feta-nummer 8" . -1) . "feta-nummer8")
    (("medium upright number feta-nummer 7" . -2) . "feta-nummer7")
    (("medium upright number feta-nummer 6" . -3) . "feta-nummer6")
    (("medium upright number feta-nummer 5" . -4) . "feta-nummer5")
    (("medium upright number feta-nummer 4" . -5) . "feta-nummer4")
    (("medium upright roman cmr 8" . -3) . "cmr8" )
    (("medium upright roman cmr 8" . -2) . "cmr8" )
    (("medium upright roman cmr 8" . -1) . "cmr8" )
    (("medium upright roman cmr 10" . 0) . "cmr10")
    (("medium upright roman cmr 12" . 1) . "cmr12")
    (("bold upright roman cmbx 10" . 0) . "cmbx10")
    (("bold upright roman cmbx 12" . 1) . "cmbx12")
    (("medium italic roman cmbx 10" . 0) . "cmbx10")
    (("medium italic roman cmbx 12" . 1) . "cmbx12")
    (("medium upright math msam 10" . -3) . "msam10")
    (("medium upright math msam 10" . -2) . "msam10")
    (("medium upright math msam 10" . -1) . "msam10")
    (("medium upright math msam 10" . 0) . "msam10")
    ))

(define (style-sheet-template-entry-compile entry size)
  (cons 
   (string-append (caar entry)
		  " "
		  (number->string (- (cdar entry) size))
		  " ")
   (cdr entry)))
   
(define style-sheet-alist
  `(
    (paper11 . ,(map (lambda (x) (style-sheet-template-entry-compile x -3))
		     paper20-style-sheet-alist-template))
    (paper13 . ,(map (lambda (x) (style-sheet-template-entry-compile x -2))
		     paper20-style-sheet-alist-template))
    (paper16 . ,(map (lambda (x) (style-sheet-template-entry-compile x -1))
		     paper20-style-sheet-alist-template))
    (paper20 . ,(map (lambda (x) (style-sheet-template-entry-compile x 0))
		     paper20-style-sheet-alist-template))
    (paper23 . ,(map (lambda (x) (style-sheet-template-entry-compile x 1))
		     paper20-style-sheet-alist-template))
    (paper26 . ,(map (lambda (x) (style-sheet-template-entry-compile x 2))
		     paper20-style-sheet-alist-template))
     ))

(define (font-regexp-to-font-name paper regexp)
  (let ((style-sheet (cdr (assoc paper style-sheet-alist))))
    (let loop ((fonts style-sheet))
      (if (string-match regexp (caar fonts))
	  (cdar fonts)
	  (if (pair? (cdr fonts))
	      (loop (cdr fonts))
	      '())))))

(define (properties-to-font-name paper properties-alist)
  (let ((font-regexp (apply string-append
	 (map (lambda (key)
		(string-append
		 (let ((entry (assoc key properties-alist)))
		   (if entry (cdr entry) "[^ ]+"))
		 " "))
	      '(font-series font-shape font-family font-name font-point font-size)))))
    ;;(display "font-regexp: `")
    ;;(display font-regexp)
    ;;(display "'\n")
    (font-regexp-to-font-name paper font-regexp)))

(define markup-abbrev-to-properties-alist
  (append
   '(
     (rows . ((align . 0)))
     (lines . ((align . 1)))
     (roman . ((font-family . "roman")))
     (music . ((font-family . "music")))
     (bold . ((font-series . "bold")))
     (italic . ((font-shape . "italic")))
     (named . ((lookup . name)))
     (super . ((raise . 1) (font-size . "-1")))
     (sub . ((raise . -1) (font-size . "-1")))
     (text . ((lookup . value)))
     )
   (map (lambda (x) (cons (car x) (cons 'font-style (car x))))
	style-to-font-alist)))
  
(define (markup-to-properties markup)
  ;;(display "markup: `")
  ;;(display markup)
  ;;(display "'\n")
  (if (pair? markup)
      (list markup)
      (let ((entry (assoc markup markup-abbrev-to-properties-alist)))
	(if entry (cdr entry)
	    (list (cons markup #t))))))
	
(define (style-to-font-name paper style)
  (let* ((entry (assoc style style-to-font-alist))
	 (font (if entry (cdr entry) "* * * * * *"))
	 (font-regexp
	  (regexp-substitute/global #f "\\*" font 'pre "[^ ]+" 'post)))
    (font-regexp-to-font-name paper font-regexp)))

