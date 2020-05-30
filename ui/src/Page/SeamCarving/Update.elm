module Page.SeamCarving.Update exposing (update)

import Data.SeamCarving exposing (CarveImageParams)
import Extra.Cmd exposing (cmd, none)
import Page.SeamCarving.Model exposing (GrowForm, Model, defaultGrowForm)
import Page.SeamCarving.Msg exposing (GrowFormMsg(..), Msg(..))
import RemoteData as RD exposing (RemoteData(..), WebData)
import Request.Request exposing (healthCheck)
import Request.SeamCarving exposing (carveImage)


defaultCarveImageParams : String -> CarveImageParams
defaultCarveImageParams title =
    { imageName = title }


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        NoOp ->
            model
                |> none

        HealthCheck ->
            { model | healthCheck = Loading }
                |> cmd (\m -> healthCheck m.flags HealthChecked)

        HealthChecked resp ->
            { model | healthCheck = resp } |> none

        CarveImage title ->
            let
                params =
                    title
                        |> defaultCarveImageParams
            in
            { model | carveImageResp = Loading }
                |> cmd (\m -> carveImage m.flags CarvedImage params)

        CarvedImage resp ->
            ( { model | carveImageResp = resp }, Cmd.none )

        SelectImage imageName ->
            { model | selectedImage = Just imageName } |> none

        UnselectImage ->
            { model | selectedImage = Nothing }
                |> resetGrowForm
                |> none

        GrowFormMsg gMsg ->
            model |> updateGrowForm gMsg |> none


updateGrowForm : GrowFormMsg -> Model -> Model
updateGrowForm gMsg model =
    { model | growForm = updateGrowForm_ gMsg model.growForm }


updateGrowForm_ : GrowFormMsg -> GrowForm -> GrowForm
updateGrowForm_ gMsg form =
    case gMsg of
        ShowIntermediateSteps val ->
            { form | showIntermediateSteps = val }

        SetNumSteps numSteps ->
            { form | numSteps = numSteps }

        SetHeight height ->
            String.toInt height
                |> Maybe.map (\h -> { form | addHeight = h })
                |> Maybe.withDefault form

        SetWidth width ->
            String.toInt width
                |> Maybe.map (\w -> { form | addWidth = w })
                |> Maybe.withDefault form

        NumStepsDropdown state ->
            { form | numStepsDropdown = state }


resetGrowForm : Model -> Model
resetGrowForm model =
    { model | growForm = defaultGrowForm }
