module Page.SeamCarving.View exposing (view)

import Bootstrap.Button as Button
import Bootstrap.Card as Card
import Bootstrap.Card.Block as CardBlock
import Bootstrap.Utilities.Flex as Flex
import Bootstrap.Utilities.Spacing as Spacing
import Extra.Html as EH
import Flags exposing (Flags)
import Html exposing (..)
import Html.Attributes exposing (..)
import Page.SeamCarving.Model exposing (Model, imageTitles)
import Page.SeamCarving.Msg exposing (Msg(..))
import View.WebData exposing (viewWebData)


view : Model -> Html Msg
view model =
    div []
        [ viewImages
        , viewImage model.flags
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


viewImage : Flags -> Html Msg
viewImage flags =
    let
        imgSrc title =
            flags.bucket ++ "/defaults/" ++ title ++ ".png"
    in
    imageTitles
        |> List.map
            (\title ->
                Card.config []
                    |> Card.header [] [ text title ]
                    |> Card.block []
                        [ CardBlock.custom <|
                            img [ src <| imgSrc title, width 300 ] []
                        ]
                    |> Card.footer [] []
                    |> Card.view
            )
        |> div [ Flex.block ]
