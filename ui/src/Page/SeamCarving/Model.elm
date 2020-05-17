module Page.SeamCarving.Model exposing (Model, imageTitles, init)

import Page.SeamCarving.Msg exposing (Msg(..))


type alias Model =
    {}


init : ( Model, Cmd Msg )
init =
    ( {}, Cmd.none )


imageTitles : List String
imageTitles =
    [ "dolphin", "edo", "fuji", "castle" ]
