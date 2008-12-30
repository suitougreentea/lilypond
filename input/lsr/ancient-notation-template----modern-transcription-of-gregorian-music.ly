%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.12.0"

\header {
  lsrtags = "ancient-notation, template"

  texidoces = "
Este ejemplo muestra cómo hacer una transcripción moderna de canto
gregoriano. El canto gregoriano no tiene compás ni plicas; utiliza
solamente cabezas de nota de blanca y de negra, y unas marcas
especiales que indican silencios de distintas longitudes.

"

  doctitlees = "Plantilla para notación de música antigua (transcripción moderna de canto gregoriano)"
  
  texidocde = "
Dieses Beispiel zeigt eine moderne Transkription des Gregorianischen 
Chorals. Hier gibt es keine Takte, keine Notenhälse und es werden nur 
halbe und Viertelnoten verwendet. Zusätzliche Zeichen zeigen die 
Länge von Pausen an.
"

  texidoc = "
This example demonstrates how to do modern transcription of Gregorian
music. Gregorian music has no measure, no stems; it uses only half and
quarter note heads, and special marks, indicating rests of different
length.

"
  doctitle = "Ancient notation template -- modern transcription of gregorian music"
} % begin verbatim

\include "gregorian.ly"

chant = \relative c' {
  \set Score.timing = ##f
  f4 a2 \divisioMinima
  g4 b a2 f2 \divisioMaior
  g4( f) f( g) a2 \finalis
}

verba = \lyricmode {
  Lo -- rem ip -- sum do -- lor sit a -- met
}

\score {
  \new Staff <<
    \new Voice = "melody" \chant
    \new Lyrics = "one" \lyricsto melody \verba
  >>
  \layout {
    \context {
      \Staff
      \remove "Time_signature_engraver"
      \remove "Bar_engraver"
      \override Stem #'transparent = ##t
    }
    \context {
      \Voice
      \override Stem #'length = #0
    }
    \context {
      \Score
      barAlways = ##t
    }
  }
}
