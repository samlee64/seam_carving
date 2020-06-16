module Page.SeamCarving.Model exposing (..)

import Bootstrap.Dropdown as Dropdown
import Bootstrap.Tab as Tab
import Data.Markings exposing (..)
import Data.Mouse exposing (..)
import Data.SeamCarving exposing (..)
import Data.Triangle as Triangle exposing (Triangle)
import Flags exposing (Flags)
import Page.SeamCarving.Msg exposing (..)
import RemoteData as RD exposing (RemoteData(..), WebData)


type alias Model =
    { flags : Flags
    , healthCheck : WebData String
    , selectedImage : Maybe String
    , growForm : GrowForm
    , growImageResp : WebData GrowImageResp
    , pollExecutionId : Maybe String
    , pollExecutionStatusResp : WebData ExecutionStatusResp
    , contentAmplificationForm : ContentAmplificationForm
    , contentAmplificationResp : WebData ContentAmplificationResp
    , tabState : Tab.State
    , removeObjectForm : RemoveObjectForm
    , removeObjectResp : WebData RemoveObjectResp
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
            , pollExecutionId = Nothing
            , pollExecutionStatusResp = NotAsked
            , contentAmplificationForm = defaultContentAmplificationForm
            , contentAmplificationResp = NotAsked
            , tabState = Tab.initialState
            , removeObjectForm = defaultRemoveObjectForm
            , removeObjectResp = NotAsked
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


type alias ContentAmplificationForm =
    { showIntermediateSteps : Bool
    , factor : Float
    , factorDropdown : Dropdown.State
    }


defaultContentAmplificationForm : ContentAmplificationForm
defaultContentAmplificationForm =
    { showIntermediateSteps = False
    , factor = 1.2
    , factorDropdown = Dropdown.initialState
    }


type alias RemoveObjectForm =
    { protected : List Triangle
    , destroy : List Triangle
    , clickMode : ClickMode
    , markMode : MarkMode
    , showIntermediateSteps : Bool
    , mouseMoveData : Maybe MouseMoveData
    , currTriangle : Triangle
    , trianglePointIdx : Int
    , lockRatio : Bool
    , onlyHorizontal : Bool
    , onlyVertical : Bool
    , markings : Markings
    }


defaultRemoveObjectForm : RemoveObjectForm
defaultRemoveObjectForm =
    { protected = []
    , destroy = []
    , clickMode = Discreet
    , markMode = Protect
    , showIntermediateSteps = True
    , mouseMoveData = Nothing
    , currTriangle = Triangle.empty
    , trianglePointIdx = 0
    , lockRatio = False
    , onlyHorizontal = False
    , onlyVertical = False
    , markings = { destroy = "", protect = "" }
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


extractRemoveObjectParams : Model -> Maybe RemoveObjectParams
extractRemoveObjectParams ({ removeObjectForm } as model) =
    Maybe.map
        (\si ->
            { imageName = si
            , showIntermediateSteps = removeObjectForm.showIntermediateSteps
            , lockRatio = removeObjectForm.lockRatio
            , onlyHorizontal = removeObjectForm.onlyHorizontal
            , onlyVertical = removeObjectForm.onlyVertical
            , markings = removeObjectForm.markings
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
