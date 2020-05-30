module Page.SeamCarving.Model exposing (GrowForm, Model, defaultGrowForm, gifTypes, imageTitles, init, numSteps)

import Bootstrap.Dropdown as Dropdown
import Data.SeamCarving exposing (CarveImageResp)
import Flags exposing (Flags)
import Page.SeamCarving.Msg exposing (Msg(..))
import RemoteData as RD exposing (RemoteData(..), WebData)


type alias Model =
    { flags : Flags
    , healthCheck : WebData String
    , carveImageResp : WebData CarveImageResp
    , selectedImage : Maybe String
    , growForm : GrowForm
    }


init : Flags -> ( Model, Cmd Msg )
init flags =
    let
        model =
            { flags = flags
            , healthCheck = NotAsked
            , carveImageResp = NotAsked
            , selectedImage = Just "castle-medium"
            , growForm = defaultGrowForm
            }
    in
    ( model, Cmd.none )


type alias GrowForm =
    { showIntermediateSteps : Bool
    , numSteps : Int
    , addHeight : Int
    , addWidth : Int
    , numStepsDropdown : Dropdown.State
    }


defaultGrowForm : GrowForm
defaultGrowForm =
    { showIntermediateSteps = False
    , numSteps = 2
    , addHeight = 0
    , addWidth = 100
    , numStepsDropdown = Dropdown.initialState
    }


imageTitles : List String
imageTitles =
    [ "dolphin", "edo", "Fuji", "castle-medium", "beach" ]


gifTypes : List String
gifTypes =
    [ "energy-map", "mask", "mid" ]



--This one is currently not used, maybe i do want to use it through


outputFiles : List String
outputFiles =
    [ "energy-map.gif", "mask.gif", "mid.gif", "output.gif" ]


numSteps : List Int
numSteps =
    List.range 1 8
