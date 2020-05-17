module Page.SeamCarving.Update exposing (update)

import Page.SeamCarving.Model exposing (Model)
import Page.SeamCarving.Msg exposing (Msg(..))


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        NoOp ->
            ( model, Cmd.none )
