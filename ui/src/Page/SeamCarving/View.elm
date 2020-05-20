module Page.SeamCarving.View exposing (view)

import Bootstrap.Button as Button
import Bootstrap.Utilities.Spacing as Spacing
import Extra.Html as EH
import Html exposing (..)
import Html.Attributes exposing (..)
import Page.SeamCarving.Model exposing (Model, imageTitles)
import Page.SeamCarving.Msg exposing (Msg(..))
import View.WebData exposing (viewWebData)


view : Model -> Html Msg
view model =
    div []
        [ viewImages
        , viewStatus model
        , viewHealthCheck model
        ]


viewImages : Html Msg
viewImages =
    imageTitles
        |> List.map
            (\title ->
                Button.button
                    [ Button.attrs [ Spacing.ml5 ], Button.primary, Button.onClick (CarveImage title) ]
                    [ text title ]
            )
        |> div []


viewStatus : Model -> Html Msg
viewStatus model =
    model.carveImageResp
        |> viewWebData (\_ -> EH.none)


viewHealthCheck : Model -> Html Msg
viewHealthCheck model =
    div []
        [ Button.button [ Button.primary, Button.onClick HealthCheck ] [ text "Check health" ]
        , model.healthCheck |> viewWebData (\_ -> text "You are healthy")
        ]
