module Main.View exposing (..)

import Browser exposing (Document)
import Html exposing (..)
import Main.Model exposing (Model, Page(..))
import Main.Msg exposing (Msg(..))
import Page.Index as Index


view : Model -> Document Msg
view model =
    { title = ""
    , body = [ viewPage model ]
    }


viewPage : Model -> Html Msg
viewPage model =
    case model.page of
        IndexPage indexModel ->
            Html.map IndexMsg <| Index.view indexModel

        NotFound ->
            div [] [ text "Not Found" ]
