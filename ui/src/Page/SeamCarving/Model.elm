module Page.SeamCarving.Model exposing (..)

import Bootstrap.Dropdown as Dropdown
import Bootstrap.Tab as Tab
import Data.Mouse exposing (..)
import Data.SeamCarving exposing (..)
import Data.Triangle as Triangle exposing (Triangle)
import Flags exposing (Flags)
import Page.SeamCarving.Msg exposing (Msg(..))
import RemoteData as RD exposing (RemoteData(..), WebData)


type alias Model =
    { flags : Flags
    , healthCheck : WebData String
    , selectedImage : Maybe String
    , growForm : GrowForm
    , growImageResp : WebData GrowImageResp
    , pollExecutionStatusResp : WebData ExecutionStatusResp
    , contentAmplificationForm : ContentAmplificationForm
    , contentAmplificationResp : WebData ContentAmplificationResp
    , tabState : Tab.State
    , removeObjectForm : RemoveObjectForm
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
            , pollExecutionStatusResp = NotAsked
            , contentAmplificationForm = defaultContentAmplificationForm
            , contentAmplificationResp = NotAsked
            , tabState = Tab.initialState
            , removeObjectForm = defaultRemoveObjectForm
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


type alias ContentAmplificationForm =
    { showIntermediateSteps : Bool
    , factor : Float
    , factorDropdown : Dropdown.State
    }


type ClickMode
    = Protect
    | Destroy


type alias RemoveObjectForm =
    { protected : Int
    , destroy : Int
    , clickMode : ClickMode
    , showIntermediateSteps : Bool
    , mouseMoveData : Maybe MouseMoveData
    , triangles : List Triangle
    , currTriangle : Triangle
    , trianglePointIdx : Int
    }


defaultGrowForm : GrowForm
defaultGrowForm =
    { showIntermediateSteps = False
    , numSteps = 2
    , addHeight = Nothing
    , addWidth = Just 100
    , numStepsDropdown = Dropdown.initialState
    }


defaultContentAmplificationForm : ContentAmplificationForm
defaultContentAmplificationForm =
    { showIntermediateSteps = False
    , factor = 1.2
    , factorDropdown = Dropdown.initialState
    }


defaultRemoveObjectForm : RemoveObjectForm
defaultRemoveObjectForm =
    { protected = 0
    , destroy = 0
    , clickMode = Destroy
    , showIntermediateSteps = True
    , mouseMoveData = Nothing
    , triangles = []
    , currTriangle = Triangle.empty
    , trianglePointIdx = 0
    }


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


extractContentAmplificationParams : Model -> Maybe ContentAmplificationParams
extractContentAmplificationParams ({ contentAmplificationForm } as model) =
    Maybe.map
        (\si ->
            { imageName = si
            , showIntermediateSteps = contentAmplificationForm.showIntermediateSteps
            , factor = contentAmplificationForm.factor
            }
        )
        model.selectedImage



--TODO this is badly named


extractTriangleCoordFromMouseData : MouseMoveData -> List Int
extractTriangleCoordFromMouseData mouseMoveData =
    [ mouseMoveData.offsetX, mouseMoveData.offsetY ]


isExecutionDone : Model -> Bool
isExecutionDone model =
    model.pollExecutionStatusResp
        |> RD.map (\r -> r.status == Done)
        |> RD.withDefault False


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


factorRange : List Float
factorRange =
    List.range 11 20
        |> List.map (\modifier -> toFloat modifier / 10)
