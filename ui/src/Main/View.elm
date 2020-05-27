module Main.View exposing (..)

import Bootstrap.Utilities.Spacing as Spacing
import Browser exposing (Document)
import Html exposing (..)
import Html.Attributes exposing (style)
import Main.Model exposing (Model, Page(..))
import Main.Msg exposing (Msg(..))
import Page.Index as Index
import Page.SeamCarving.View as SeamCarving


view : Model -> Document Msg
view model =
    { title = ""
    , body = [ view_ model ]
    }


view_ : Model -> Html Msg
view_ model =
    div
        [ style "position" "aboslute"
        , style "height" "100vh"
        , style "width" "100vw"
        , Spacing.p5
        ]
        [ viewPage model ]


viewPage : Model -> Html Msg
viewPage model =
    case model.page of
        IndexPage imodel ->
            Html.map IndexMsg <| Index.view imodel

        SeamCarvingPage scmodel ->
            Html.map SeamCarvingMsg <| SeamCarving.view scmodel

        NotFound ->
            div [] [ text "Not Found" ]
