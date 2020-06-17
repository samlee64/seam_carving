module Page.SeamCarving.Update exposing (update)

import Data.SeamCarving exposing (..)
import Data.Triangle as Triangle
import Extra.Cmd exposing (cmd, none)
import Extra.Extra as Extra
import Extra.Maybe exposing (isJust, isNothing)
import Page.SeamCarving.Model exposing (..)
import Page.SeamCarving.Msg exposing (..)
import RemoteData as RD exposing (RemoteData(..), WebData)
import Request.Request exposing (healthCheck)
import Request.SeamCarving exposing (contentAmplification, getExecutionStatus, growImage, removeObject)


update : Msg -> Model -> ( Model, Cmd Msg )
update msg ({ flags } as model) =
    case msg of
        NoOp ->
            model
                |> none

        SelectImage imageName ->
            { model | selectedImage = Just imageName } |> none

        UnselectImage ->
            model
                |> unselectImage
                |> none

        GrowImage ->
            let
                growImageCmd m =
                    extractGrowImageParams m
                        |> Maybe.map (growImage flags GrewImage)
                        |> Maybe.withDefault Cmd.none
            in
            { model | growImageResp = Loading, pollExecutionStatusResp = Loading }
                |> cmd growImageCmd

        GrewImage resp ->
            { model | growImageResp = resp }
                |> setPollExecutionId resp
                |> none

        GrowFormMsg gMsg ->
            model
                |> updateGrowForm gMsg
                |> none

        Tick _ ->
            let
                pollCmd m =
                    m.pollExecutionId
                        |> Maybe.map (\i -> getExecutionStatus flags PolledExecutionStatus { executionId = i })
                        |> Maybe.withDefault Cmd.none
            in
            model |> cmd pollCmd

        PolledExecutionStatus resp ->
            { model | pollExecutionStatusResp = resp } |> none

        ContentAmplificationFormMsg caMsg ->
            model
                |> updateContentAmplificationForm caMsg
                |> none

        AmplifyImage ->
            let
                amplifyImageCmd m =
                    extractContentAmplificationParams m
                        |> Maybe.map (contentAmplification flags AmplifiedImage)
                        |> Maybe.withDefault Cmd.none
            in
            { model | contentAmplificationResp = Loading, pollExecutionStatusResp = Loading }
                |> cmd amplifyImageCmd

        AmplifiedImage resp ->
            --TODO fire off a poll cmd here
            { model | contentAmplificationResp = resp }
                |> setPollExecutionId resp
                |> none

        TabMsg state ->
            { model | tabState = state }
                |> resetPollExecution
                |> none

        RemoveObjectFormMsg rMsg ->
            model
                |> updateRemoveObjectForm rMsg
                |> none

        RemovedObject resp ->
            let
                log =
                    Debug.log "RemovedObject" resp
            in
            --TODO fire off a poll cmd
            { model | removeObjectResp = Success resp }
                |> setPollExecutionId (Success resp)
                |> none

        GotInputImages resp ->
            { model | inputImagesResp = resp }
                |> none


updateGrowForm : GrowFormMsg -> Model -> Model
updateGrowForm gMsg model =
    { model | growForm = updateGrowForm_ gMsg model.growForm }


updateGrowForm_ : GrowFormMsg -> GrowForm -> GrowForm
updateGrowForm_ gMsg form =
    case gMsg of
        ShowGrowIntermediateSteps val ->
            { form | showIntermediateSteps = val }

        SetNumSteps numSteps ->
            { form | numSteps = numSteps }

        SetHeight height ->
            if String.isEmpty height then
                { form | addHeight = Nothing }

            else
                String.toInt height
                    |> Maybe.map (\h -> { form | addHeight = Just h })
                    |> Maybe.withDefault form

        SetWidth width ->
            if String.isEmpty width then
                { form | addWidth = Nothing }

            else
                String.toInt width
                    |> Maybe.map (\w -> { form | addWidth = Just w })
                    |> Maybe.withDefault form

        NumStepsDropdown state ->
            { form | numStepsDropdown = state }


updateContentAmplificationForm : ContentAmplificationFormMsg -> Model -> Model
updateContentAmplificationForm caMsg model =
    { model | contentAmplificationForm = updateContentAmplificationForm_ caMsg model.contentAmplificationForm }


updateContentAmplificationForm_ : ContentAmplificationFormMsg -> ContentAmplificationForm -> ContentAmplificationForm
updateContentAmplificationForm_ caMsg form =
    case caMsg of
        ShowContentAmplificationIntermediateSteps val ->
            { form | showIntermediateSteps = val }

        SetFactor factor ->
            { form | factor = factor }

        FactorDropdown state ->
            { form | factorDropdown = state }


updateRemoveObjectForm : RemoveObjectFormMsg -> Model -> Model
updateRemoveObjectForm rMsg model =
    { model | removeObjectForm = updateRemoveObjectForm_ rMsg model.removeObjectForm }


updateRemoveObjectForm_ : RemoveObjectFormMsg -> RemoveObjectForm -> RemoveObjectForm
updateRemoveObjectForm_ rMsg form =
    case rMsg of
        ShowRemoveObjectIntermediateSteps val ->
            { form | showIntermediateSteps = val }

        SetClickMode mode ->
            { form | clickMode = mode }

        SetMarkMode mode ->
            { form | markMode = mode }

        MouseMove data ->
            { form | mouseMoveData = Just data }

        --                |> (\f -> Maybe.map extractTriangleCoordFromMouseData f.mouseMoveData)
        --                |> Maybe.map (\coord -> Triangle.updateLatest form.currTriangle coord)
        --                |> Maybe.withDefault form.currTriangle
        --                |> (\tri -> { form | currTriangle = tri })
        Click ->
            let
                tri =
                    form.currTriangle

                nextTriangle t =
                    if form.clickMode == Continious then
                        Triangle.shiftRight t []

                    else
                        Triangle.empty

                updateProper item ff =
                    case form.markMode of
                        Protect ->
                            { ff | protected = item :: form.protected }

                        Destroy ->
                            { ff | destroy = item :: form.destroy }
            in
            form.mouseMoveData
                |> Maybe.map extractTriangleCoordFromMouseData
                |> Maybe.map
                    (\coord ->
                        Triangle.addCoord tri coord
                            |> Result.map
                                (\updatedTri ->
                                    if Triangle.isComplete updatedTri then
                                        { form | currTriangle = nextTriangle updatedTri }
                                            |> updateProper updatedTri

                                    else
                                        { form | currTriangle = updatedTri }
                                )
                            --triangle is complete
                            |> Result.withDefault form
                     --({ form | currTriangle = Triangle.shiftRight tri [] } |> updateProper tri)
                    )
                |> Maybe.withDefault form

        SetLockRatio val ->
            { form | lockRatio = val }

        SetOnlyHorizontal val ->
            { form | onlyHorizontal = val }

        SetOnlyVertical val ->
            { form | onlyVertical = val }

        AccordionMsg state ->
            { form | showTriangleData = state }


unselectImage : Model -> Model
unselectImage model =
    { model
        | selectedImage = Nothing
        , pollExecutionStatusResp = NotAsked
        , growImageResp = NotAsked
        , contentAmplificationResp = NotAsked
        , removeObjectResp = NotAsked
    }
        |> resetGrowForm
        |> resetContentAmplificationForm
        |> resetRemoveObjectForm
        |> resetPollExecution


resetGrowForm : Model -> Model
resetGrowForm model =
    { model | growForm = defaultGrowForm }


resetContentAmplificationForm : Model -> Model
resetContentAmplificationForm model =
    { model | contentAmplificationForm = defaultContentAmplificationForm }


resetRemoveObjectForm : Model -> Model
resetRemoveObjectForm model =
    { model | removeObjectForm = defaultRemoveObjectForm }


setPollExecutionId : WebData { a | executionId : String } -> Model -> Model
setPollExecutionId resp model =
    resp
        |> RD.toMaybe
        |> Maybe.map .executionId
        |> (\i -> { model | pollExecutionId = i })


resetPollExecution : Model -> Model
resetPollExecution model =
    { model | pollExecutionStatusResp = NotAsked, pollExecutionId = Nothing }



--pollExecutionStatus : Model -> ( Model, Cmd Msg )
--pollExecutionStatus model =
--    model.growImageResp
--        |> RD.toMaybe
--        |> Maybe.map (\gi -> { executionId = gi.executionId })
--        |> Maybe.map (getExecutionStatus model.flags PolledExecutionStatus)
--        |> Maybe.map (\c -> ( { model | pollExecutionStatusResp = Loading }, c ))
--        |> Maybe.withDefault ( model, Cmd.none )
