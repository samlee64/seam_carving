module Page.SeamCarving.Update exposing (update)

import Extra.Cmd exposing (cmd, none)
import Page.SeamCarving.Model exposing (..)
import Page.SeamCarving.Msg exposing (GrowFormMsg(..), Msg(..))
import RemoteData as RD exposing (RemoteData(..), WebData)
import Request.Request exposing (healthCheck)
import Request.SeamCarving exposing (growImage)


update : Msg -> Model -> ( Model, Cmd Msg )
update msg ({ flags } as model) =
    case msg of
        NoOp ->
            model
                |> none

        HealthCheck ->
            { model | healthCheck = Loading }
                |> cmd (\_ -> healthCheck flags HealthChecked)

        HealthChecked resp ->
            { model | healthCheck = resp } |> none

        SelectImage imageName ->
            { model | selectedImage = Just imageName } |> none

        UnselectImage ->
            { model | selectedImage = Nothing }
                |> resetGrowForm
                |> none

        GrowImage ->
            let
                params =
                    model.growForm
            in
            model
                |> cmd
                    (\m ->
                        extractGrowImageParams m
                            |> Maybe.map (growImage flags GrewImage)
                            |> Maybe.withDefault Cmd.none
                    )

        GrewImage resp ->
            { model | growImageResp = resp } |> none

        GrowFormMsg gMsg ->
            model
                |> updateGrowForm gMsg
                |> none


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


resetGrowForm : Model -> Model
resetGrowForm model =
    { model | growForm = defaultGrowForm }
