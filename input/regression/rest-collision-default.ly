#(ly:set-option 'old-relative)
\version "1.9.8"

\header
{
texidoc = "Rests in collisions sit opposite of the note if no direction is
  specified for the voice containing the rest."
}


    \paper { raggedright= ##t }

\score{\notes\relative c''
       \context Staff <<
\time 2/4
\new Voice {
    r4 }
\new Voice {\voiceTwo
b8  }>>}

