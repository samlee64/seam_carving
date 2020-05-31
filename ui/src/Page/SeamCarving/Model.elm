module Page.SeamCarving.Model exposing (..)

import Bootstrap.Dropdown as Dropdown
import Data.SeamCarving exposing (..)
import Flags exposing (Flags)
import Page.SeamCarving.Msg exposing (Msg(..))
import RemoteData as RD exposing (RemoteData(..), WebData)


type alias Model =
    { flags : Flags
    , healthCheck : WebData String
    , selectedImage : Maybe String
    , growForm : GrowForm
    , growImageResp : WebData GrowImageResp
    }


init : Flags -> ( Model, Cmd Msg )
init flags =
    let
        model =
            { flags = flags
            , healthCheck = NotAsked
            , selectedImage = Just "dolphin"
            , growForm = defaultGrowForm
            , growImageResp = NotAsked
            }
    in
    ( model, Cmd.none )


type alias GrowForm =
    { showIntermediateSteps : Bool
    , numSteps : Int
    , addHeight : Maybe Int
    , addWidth : Maybe Int
    , numStepsDropdown : Dropdown.State
    }


defaultGrowForm : GrowForm
defaultGrowForm =
    { showIntermediateSteps = False
    , numSteps = 2
    , addHeight = Nothing
    , addWidth = Just 100
    , numStepsDropdown = Dropdown.initialState
    }


isGrowFormValid : GrowForm -> Bool
isGrowFormValid form =
    True


extractGrowImageParams : Model -> Maybe GrowImageParams
extractGrowImageParams ({ growForm } as model) =
    Maybe.map
        (\si ->
            { imageName = si
            , showIntermediateSteps = growForm.showIntermediateSteps
            , numSteps = growForm.numSteps
            , addHeight = Maybe.withDefault 0 growForm.addHeight
            , addWidth = Maybe.withDefault 0 growForm.addWidth
            }
        )
        model.selectedImage


imageTitles : List String
imageTitles =
    [ "dolphin", "edo", "Fuji", "castle-medium", "beach" ]


gifTypes : List String
gifTypes =
    [ "energy-map", "mask", "mid" ]



--This one is currently not used, maybe i do want to use it through


outputFiles : List String
outputFiles =
    [ "energy-map.gif", "mask.gif", "mid.gif", "output.png" ]


numSteps : List Int
numSteps =
    List.range 1 8
