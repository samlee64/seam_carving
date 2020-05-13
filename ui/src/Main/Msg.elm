module Main.Msg exposing (Msg(..))

import Browser exposing (UrlRequest)
import Page.Index as Index
import Url exposing (Url)


type Msg
    = UrlChange Url
    | UrlRequest UrlRequest
    | IndexMsg Index.Msg
