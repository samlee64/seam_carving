module Main.Msg exposing (Msg(..))

import Bootstrap.Navbar as Navbar
import Browser exposing (UrlRequest)
import Page.Index as Index
import Page.SeamCarving.Msg as SeamCarving
import Page.Technologies as Technologies
import Url exposing (Url)


type Msg
    = UrlChange Url
    | UrlRequest UrlRequest
    | IndexMsg Index.Msg
    | SeamCarvingMsg SeamCarving.Msg
    | TechnologiesMsg Technologies.Msg
    | NavbarMsg Navbar.State
