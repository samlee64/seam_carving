module Page.SeamCarving.Model exposing (Model, imageTitles, init)

import Data.SeamCarving exposing (CarveImageResp)
import Flags exposing (Flags)
import Page.SeamCarving.Msg exposing (Msg(..))
import RemoteData as RD exposing (RemoteData(..), WebData)


type alias Model =
    { flags : Flags
    , healthCheck : WebData String
    , carveImageResp : WebData CarveImageResp
    }


init : Flags -> ( Model, Cmd Msg )
init flags =
    let
        model =
            { flags = flags
            , healthCheck = NotAsked
            , carveImageResp = NotAsked
            }
    in
    ( model, Cmd.none )


imageTitles : List String
imageTitles =
    [ "dolphin", "edo", "Fuji", "castle", "beach" ]
