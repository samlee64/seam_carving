module Main.Update exposing (update)

import Browser
import Browser.Navigation as Navigation
import Main.Model exposing (Model, Page(..), initPage, updatePage)
import Main.Msg exposing (Msg(..))
import Page.Index as Index


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case ( msg, model.page ) of
        ( UrlRequest urlRequest, _ ) ->
            --TODO(sam)
            ( model, Cmd.none )

        ( UrlChange url, _ ) ->
            --TODO(sam)
            ( model, Cmd.none )

        ( IndexMsg imsg, IndexPage imodel ) ->
            Index.update imsg imodel |> updatePage IndexPage IndexMsg model

        _ ->
            ( model, Cmd.none )
