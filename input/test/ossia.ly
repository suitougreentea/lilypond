\version "1.7.18"
% pretty much a duplicate of extra-staff.ly. candidate for merge or delete. -gp

\header { texidoc = "@cindex Ossia
Ossias present alternatives for a piece. They are not
really supported, but can be somewhat faked in lily. "
}

\score {
\notes { c1 c1 <
	{\clef "treble" c1 c1 }
	\context Staff = stb { c1 c1 }
	>
}
	\paper {raggedright= ##t}
}
%% new-chords-done %%
