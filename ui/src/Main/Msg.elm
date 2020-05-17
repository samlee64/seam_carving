module Main.Msg exposing (Msg(..))

import Browser exposing (UrlRequest)
import Page.Index as Index
import Page.SeamCarving.Msg as SeamCarving
import Url exposing (Url)


type Msg
    = UrlChange Url
    | UrlRequest UrlRequest
    | IndexMsg Index.Msg
    | SeamCarvingMsg SeamCarving.Msg
