\version "1.3.146"

one =  \notes\relative c{
	c'' d e f
}

two =  \notes\relative c{
	\clef "bass"
	c'2 g2
}

\score{
	<
		\one
		\two
	>
	\paper{}
	\midi{}
}

% A full example with two staffs
%
% Type:
%
%     ly2dvi example-3
%     xdvi example-3     # or your dvi viewer here
