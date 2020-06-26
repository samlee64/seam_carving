module Page.SeamCarving.View exposing (view)

import Bool.Extra as BE
import Bootstrap.Accordion as Accordion
import Bootstrap.Alert as Alert
import Bootstrap.Badge as Badge
import Bootstrap.Button as Button
import Bootstrap.Card as Card
import Bootstrap.Card.Block as CardBlock
import Bootstrap.Dropdown as Dropdown
import Bootstrap.Form as Form
import Bootstrap.Form.Checkbox as Checkbox
import Bootstrap.Form.Input as Input
import Bootstrap.Tab as Tab
import Bootstrap.Utilities.Flex as Flex
import Bootstrap.Utilities.Spacing as Spacing
import Data.Mouse exposing (..)
import Data.SeamCarving exposing (..)
import Data.Triangle as Triangle exposing (Triangle)
import Extra.Extra as Extra
import Extra.Html as EH
import Flags exposing (Flags)
import Html exposing (..)
import Html.Attributes exposing (..)
import Html.Events exposing (on, onClick)
import Json.Decode as Decode
import Json.Encode as E
import List.Extra as LE
import Page.SeamCarving.ContentAmplificationFormView exposing (viewContentAmplificationForm)
import Page.SeamCarving.GrowFormView exposing (viewGrowForm)
import Page.SeamCarving.Model exposing (..)
import Page.SeamCarving.Msg exposing (..)
import Page.SeamCarving.RemoveObjectFormView exposing (viewObjectRemovalForm)
import RemoteData as RD exposing (RemoteData(..), WebData)
import Tuple
import View.WebData exposing (viewWebData, viewWebDataButton)


view : Model -> Html Msg
view model =
    div []
        [ viewToolbar model
        , model.selectedImage
            |> Maybe.map (viewSelectedImage model)
            |> Maybe.withDefault (viewAllImages model)
        ]


viewToolbar : Model -> Html Msg
viewToolbar model =
    div [ Spacing.mb2 ]
        [ model.selectedImage
            |> Maybe.map (\_ -> Button.button [ Button.primary, Button.onClick UnselectImage ] [ text "Back" ])
            |> Maybe.withDefault EH.none
        ]


viewAllImages : Model -> Html Msg
viewAllImages model =
    let
        imgSrc bucket key =
            model.flags.bucket ++ "/" ++ key

        getImageName key =
            String.split "/" key |> LE.last |> Maybe.withDefault key

        viewImage bucket key =
            Card.config [ Card.attrs [ onClick <| SelectImage (imgSrc bucket key), Spacing.m2, style "cursor" "pointer" ] ]
                |> Card.header [] [ text <| getImageName key ]
                |> Card.block []
                    [ CardBlock.custom <| img [ src (imgSrc bucket key) ] []
                    ]
                |> Card.footer [] []
                |> Card.view
    in
    model.inputImagesResp
        |> viewWebData
            (\r ->
                r.keys
                    |> List.map (viewImage r.bucket)
                    |> div [ Flex.block, Flex.wrap ]
            )


viewSelectedImage : Model -> String -> Html Msg
viewSelectedImage model imageTitle =
    Tab.config TabMsg
        |> Tab.withAnimation
        |> Tab.items
            [ Tab.item
                { id = "object-removal"
                , link = Tab.link [] [ text "Object Removal" ]
                , pane = Tab.pane [ Spacing.mt3 ] [ viewObjectRemoval model ]
                }
            , Tab.item
                { id = "content-amplification"
                , link = Tab.link [] [ text "Content Amplification" ]
                , pane = Tab.pane [ Spacing.mt3 ] [ viewContentAmplification model ]
                }
            , Tab.item
                { id = "grow"
                , link = Tab.link [] [ text "Grow" ]
                , pane = Tab.pane [ Spacing.mt3 ] [ viewGrow model ]
                }
            ]
        |> Tab.view model.tabState


viewGrow : Model -> Html Msg
viewGrow model =
    div []
        [ viewGrowForm model
        , viewResults model
        ]


viewContentAmplification : Model -> Html Msg
viewContentAmplification model =
    div []
        [ viewContentAmplificationForm model
        , viewResults model
        ]


viewObjectRemoval : Model -> Html Msg
viewObjectRemoval model =
    div []
        [ viewObjectRemovalForm model
        , viewResults model
        ]


viewExecutionStatus : Model -> Html Msg
viewExecutionStatus model =
    let
        message status =
            text <| statusToString status

        alert status =
            case status of
                Executing ->
                    Alert.simpleInfo [] [ message status ]

                Uploading ->
                    Alert.simpleInfo [] [ message status ]

                Done ->
                    Alert.simpleSuccess [] [ message status ]

                Error ->
                    Alert.simpleDanger [] [ message status ]
    in
    model.pollExecutionStatusResp
        |> viewWebData (\r -> alert r.status)


viewResults : Model -> Html Msg
viewResults model =
    let
        imgSrc s3Key =
            model.flags.bucket ++ "/" ++ s3Key

        viewResult s3Key =
            isOutputImage s3Key
                |> Extra.ternary
                    (Card.config []
                        |> Card.header [] [ text "Output" ]
                        |> Card.block [] [ CardBlock.custom <| img [ src (imgSrc s3Key) ] [] ]
                        |> Card.view
                    )
                    (img [ src (imgSrc s3Key) ] [])

        imageKeys =
            model.pollExecutionStatusResp
                |> RD.toMaybe
                |> Maybe.andThen .s3Url
                |> Maybe.map (List.partition isOutputImage)

        viewImage s3Key =
            div [ Spacing.m1 ]
                [ img [ src (imgSrc s3Key) ] []
                , div [] [ a [ href <| imgSrc s3Key ] [ text "Download" ] ]
                ]

        viewOutputImage =
            imageKeys
                |> Maybe.map Tuple.first
                |> Maybe.map
                    (\o ->
                        Card.config [ Card.attrs [ Spacing.mt3 ] ]
                            |> Card.header [] [ text "Output" ]
                            |> Card.block []
                                [ List.map viewImage o
                                    |> div []
                                    |> CardBlock.custom
                                ]
                            |> Card.view
                    )
                |> Maybe.withDefault EH.none

        viewRest =
            imageKeys
                |> Maybe.map Tuple.second
                |> Maybe.map
                    (\r ->
                        Card.config [ Card.attrs [ Spacing.mt3 ] ]
                            |> Card.header [] [ text "Intermediate Steps" ]
                            |> Card.block []
                                [ List.map viewImage r
                                    |> div []
                                    |> CardBlock.custom
                                ]
                            |> Card.view
                    )
                |> Maybe.withDefault EH.none
    in
    div []
        [ viewExecutionStatus model
        , model.pollExecutionStatusResp
            |> RD.map (\_ -> div [ Flex.block, Flex.wrap ] [ viewOutputImage, viewRest ])
            |> RD.withDefault EH.none
        ]
