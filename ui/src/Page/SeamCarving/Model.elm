module Page.SeamCarving.Model exposing (..)

import Bootstrap.Accordion as Accordion
import Bootstrap.Dropdown as Dropdown
import Bootstrap.Tab as Tab
import Data.Mouse exposing (..)
import Data.SeamCarving exposing (..)
import Data.Triangle as Triangle exposing (Triangle)
import Flags exposing (Flags)
import List.Extra as LE
import Page.SeamCarving.Msg exposing (..)
import RemoteData as RD exposing (RemoteData(..), WebData)
import Request.SeamCarving exposing (..)


type alias Model =
    { flags : Flags
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
    , inputImagesResp : WebData InputImagesResp
    }


init : Flags -> ( Model, Cmd Msg )
init flags =
    let
        model =
            { flags = flags
            , selectedImage = Nothing
            , growForm = defaultGrowForm
            , growImageResp = NotAsked
            , pollExecutionId = Nothing
            , pollExecutionStatusResp = NotAsked
            , contentAmplificationForm = defaultContentAmplificationForm
            , contentAmplificationResp = NotAsked
            , tabState = Tab.initialState
            , removeObjectForm = defaultRemoveObjectForm
            , removeObjectResp = NotAsked
            , inputImagesResp = Loading
            }
    in
    ( model, getInputImages flags GotInputImages )


type alias GrowForm =
    { showIntermediateSteps : Bool
    , numSteps : Int
    , addHeight : Maybe Int
    , addWidth : Maybe Int
    , numStepsDropdown : Dropdown.State
    , showHelp : Accordion.State
    }


defaultGrowForm : GrowForm
defaultGrowForm =
    { showIntermediateSteps = False
    , numSteps = 2
    , addHeight = Nothing
    , addWidth = Just 100
    , numStepsDropdown = Dropdown.initialState
    , showHelp = Accordion.initialStateCardOpen ""
    }


type alias ContentAmplificationForm =
    { showIntermediateSteps : Bool
    , factor : Float
    , factorDropdown : Dropdown.State
    , showHelp : Accordion.State
    }


defaultContentAmplificationForm : ContentAmplificationForm
defaultContentAmplificationForm =
    { showIntermediateSteps = False
    , factor = 1.2
    , factorDropdown = Dropdown.initialState
    , showHelp = Accordion.initialStateCardOpen ""
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
    , showTriangleData : Accordion.State
    , showHelp : Accordion.State
    }


defaultRemoveObjectForm : RemoveObjectForm
defaultRemoveObjectForm =
    { protected = []
    , destroy = []
    , clickMode = Discreet
    , markMode = Destroy
    , showIntermediateSteps = True
    , mouseMoveData = Nothing
    , currTriangle = Triangle.empty
    , trianglePointIdx = 0
    , lockRatio = False
    , onlyHorizontal = False
    , onlyVertical = False
    , showTriangleData = Accordion.initialStateCardOpen ""
    , showHelp = Accordion.initialStateCardOpen ""
    }


extractGrowImageParams : Model -> Maybe GrowImageParams
extractGrowImageParams ({ growForm } as model) =
    model
        |> getSelectedImageName
        |> Maybe.map
            (\i ->
                { imageName = i
                , showIntermediateSteps = growForm.showIntermediateSteps
                , numSteps = growForm.numSteps
                , addHeight = Maybe.withDefault 0 growForm.addHeight
                , addWidth = Maybe.withDefault 0 growForm.addWidth
                }
            )


extractContentAmplificationParams : Model -> Maybe ContentAmplificationParams
extractContentAmplificationParams ({ contentAmplificationForm } as model) =
    model
        |> getSelectedImageName
        |> Maybe.map
            (\i ->
                { imageName = i
                , showIntermediateSteps = contentAmplificationForm.showIntermediateSteps
                , factor = contentAmplificationForm.factor
                }
            )


extractTriangleCoordFromMouseData : MouseMoveData -> List Int
extractTriangleCoordFromMouseData mouseMoveData =
    [ mouseMoveData.offsetX, mouseMoveData.offsetY ]


isExecutionDone : Model -> Bool
isExecutionDone model =
    model.pollExecutionStatusResp
        |> RD.map (\r -> r.status == Done)
        |> RD.withDefault False


numSteps : List Int
numSteps =
    List.range 1 8


factorRange : List Float
factorRange =
    List.range 11 20
        |> List.map (\modifier -> toFloat modifier / 10)


getSelectedImageName : Model -> Maybe String
getSelectedImageName model =
    model.selectedImage
        |> Maybe.andThen (LE.last << String.split "/")
        |> Maybe.andThen (List.head << String.split ".")


isOutputImage : String -> Bool
isOutputImage key =
    key
        |> String.split "/"
        |> LE.last
        |> Maybe.map ((==) "output.png")
        |> Maybe.withDefault False
