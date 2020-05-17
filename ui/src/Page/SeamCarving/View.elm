module Page.SeamCarving.View exposing (view)

import Html exposing (..)
import Html.Attributes exposing (..)
import Page.SeamCarving.Model exposing (Model, imageTitles)
import Page.SeamCarving.Msg exposing (Msg(..))


view : Model -> Html Msg
view model =
    div []
        [ viewImages
        ]


viewImages : Html Msg
viewImages =
    imageTitles
        |> List.map (\title -> button [] [ text title ])
        |> div []
