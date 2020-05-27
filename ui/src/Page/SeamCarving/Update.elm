module Page.SeamCarving.Update exposing (update)

import Data.SeamCarving exposing (CarveImageParams)
import Extra.Cmd exposing (cmd, none)
import Page.SeamCarving.Model exposing (Model)
import Page.SeamCarving.Msg exposing (Msg(..))
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
            let
                log =
                    Debug.log "Resp " resp
            in
            ( { model | carveImageResp = resp }, Cmd.none )

        GetImage ->
            model |> none

        GotImage resp ->
            model |> none
