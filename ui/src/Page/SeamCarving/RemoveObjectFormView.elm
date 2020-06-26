module Page.SeamCarving.RemoveObjectFormView exposing (viewObjectRemovalForm)

import Bool.Extra as BE
import Bootstrap.Accordion as Accordion
import Bootstrap.Badge as Badge
import Bootstrap.Button as Button
import Bootstrap.Card as Card
import Bootstrap.Card.Block as CardBlock
import Bootstrap.Dropdown as Dropdown
import Bootstrap.Form as Form
import Bootstrap.Form.Checkbox as Checkbox
import Bootstrap.Form.Input as Input
import Bootstrap.Utilities.Flex as Flex
import Bootstrap.Utilities.Spacing as Spacing
import Data.Mouse exposing (..)
import Data.SeamCarving exposing (..)
import Data.Triangle as Triangle exposing (Triangle)
import Extra.Extra as Extra
import Extra.Html as EH
import Html exposing (..)
import Html.Attributes exposing (..)
import Html.Events exposing (on, onClick)
import Json.Decode as Decode
import Json.Encode as E
import Page.SeamCarving.Model exposing (..)
import Page.SeamCarving.Msg exposing (..)
import RemoteData as RD exposing (RemoteData(..), WebData)


viewObjectRemovalForm : Model -> Html Msg
viewObjectRemovalForm model =
    Card.config []
        |> Card.header [] [ text "Object Removal Form" ]
        |> Card.block []
            [ CardBlock.custom <| viewCanvasControls model
            , CardBlock.custom <| viewCanvas model
            ]
        |> Card.view


viewCanvasControls : Model -> Html Msg
viewCanvasControls ({ removeObjectForm } as model) =
    let
        clickMode =
            removeObjectForm.clickMode
                == Continious
                |> Extra.ternary
                    (Badge.badgeLight [] [ text "Continious" ])
                    (Badge.badgeDark [] [ text "Discreet" ])

        markMode =
            removeObjectForm.markMode
                == Protect
                |> Extra.ternary
                    (Badge.badgeSuccess [] [ text "Protect" ])
                    (Badge.badgeDanger [] [ text "Destroy" ])
    in
    Html.map RemoveObjectFormMsg <|
        div []
            [ viewTriangleData model
            , div [ Spacing.mt3 ]
                [ div [ Spacing.mb2 ]
                    [ div [ Spacing.mb1 ]
                        [ Button.button
                            [ Button.success
                            , Button.onClick (SetMarkMode Protect)
                            , Button.disabled <| removeObjectForm.markMode == Protect
                            ]
                            [ text "Set Protected Areas" ]
                        , Button.button
                            [ Button.danger
                            , Button.onClick (SetMarkMode Destroy)
                            , Button.disabled <| removeObjectForm.markMode == Destroy
                            ]
                            [ text "Set Destroy Areas" ]
                        ]
                    , div []
                        [ Button.button
                            [ Button.success
                            , Button.onClick (SetClickMode Continious)
                            , Button.disabled <| removeObjectForm.clickMode == Continious
                            ]
                            [ text "ClickMode: Continious" ]
                        , Button.button
                            [ Button.danger
                            , Button.onClick (SetClickMode Discreet)
                            , Button.disabled <| removeObjectForm.clickMode == Discreet
                            ]
                            [ text "ClickMode: Discreet" ]
                        ]
                    ]
                ]
            , div [ Spacing.mb2 ]
                [ Checkbox.checkbox
                    [ Checkbox.checked removeObjectForm.lockRatio
                    , Checkbox.onCheck SetLockRatio
                    ]
                    "Lock Width/Height Ratio"
                , Checkbox.checkbox
                    [ Checkbox.checked removeObjectForm.onlyHorizontal
                    , Checkbox.onCheck SetOnlyHorizontal
                    , Checkbox.disabled removeObjectForm.lockRatio
                    ]
                    "Remove only horizontal seams"
                , Checkbox.checkbox
                    [ Checkbox.checked removeObjectForm.onlyVertical
                    , Checkbox.onCheck SetOnlyVertical
                    , Checkbox.disabled removeObjectForm.lockRatio
                    ]
                    "Remove only vertical seams"
                ]
            , div []
                [ h5 [] [ text "ClickMode: ", clickMode ]
                , h5 [] [ text "MarkMode: ", markMode ]
                ]
            ]


viewCanvas : Model -> Html Msg
viewCanvas ({ removeObjectForm } as model) =
    let
        imgSrc =
            model.selectedImage
                |> Maybe.withDefault ""

        currTri =
            removeObjectForm.mouseMoveData
                |> Maybe.map extractTriangleCoordFromMouseData
                |> Maybe.andThen (\c -> Result.toMaybe <| Triangle.addCoord removeObjectForm.currTriangle c)

        protected =
            currTri
                |> Maybe.map
                    (\t ->
                        removeObjectForm.markMode
                            == Protect
                            |> Extra.ternary (t :: removeObjectForm.protected) removeObjectForm.protected
                    )
                |> Maybe.withDefault removeObjectForm.protected
                |> (\l -> E.encode 0 (E.list Triangle.encode l))

        destroy =
            currTri
                |> Maybe.map
                    (\t ->
                        removeObjectForm.markMode == Destroy |> Extra.ternary (t :: removeObjectForm.destroy) removeObjectForm.destroy
                    )
                |> Maybe.withDefault removeObjectForm.destroy
                |> (\l -> E.encode 0 (E.list Triangle.encode l))

        imageName =
            getSelectedImageName model
                |> Maybe.withDefault ""

        attributes =
            [ on "mousemove" (Decode.map MouseMove mouseMoveDataDecoder) |> Html.Attributes.map RemoveObjectFormMsg
            , on "response" (Decode.map RemovedObject removeObjectRespEventDecoder)
            , on "drawing-click" (Decode.succeed Click) |> Html.Attributes.map RemoveObjectFormMsg
            , attribute "destroy" destroy
            , attribute "imgSrc" imgSrc
            , attribute "protected" protected
            , attribute "onlyHorizontal" <| BE.toString removeObjectForm.onlyHorizontal
            , attribute "onlyVertical" <| BE.toString removeObjectForm.onlyVertical
            , attribute "lockRatio" <| BE.toString removeObjectForm.lockRatio
            , attribute "showIntermediateSteps" <| BE.toString removeObjectForm.showIntermediateSteps
            , attribute "imageName" imageName
            ]
    in
    div []
        [ node "remove-object" attributes []
        ]


viewTriangleData : Model -> Html RemoveObjectFormMsg
viewTriangleData { removeObjectForm } =
    let
        triToString data =
            E.encode 0 (E.list (E.list E.int) [ data.one, data.two, data.three ])

        viewCoords data =
            div [ Spacing.m2 ] [ text <| triToString data ]
    in
    Accordion.config TriangleDataAccordionMsg
        |> Accordion.withAnimation
        |> Accordion.cards
            [ Accordion.card
                { id = "card"
                , options = []
                , header = Accordion.header [] <| Accordion.toggle [] [ text "View Marking Data" ]
                , blocks =
                    [ Accordion.block []
                        [ CardBlock.text []
                            [ div []
                                [ h6 [] [ text "Destory Coords" ]
                                , div [ Flex.block, Flex.row, style "background" "red" ] (List.map viewCoords removeObjectForm.destroy)
                                ]
                            ]
                        ]
                    , Accordion.block []
                        [ CardBlock.text []
                            [ div []
                                [ h6 [] [ text "Protect Coords" ]
                                , div [ Flex.block, Flex.row, Flex.wrap, style "background" "grey" ] (List.map viewCoords removeObjectForm.protected)
                                ]
                            ]
                        ]
                    , Accordion.block []
                        [ CardBlock.text []
                            [ div [] [ h6 [] [ text "Current Triangle" ], viewCoords removeObjectForm.currTriangle ]
                            ]
                        ]
                    ]
                }
            ]
        |> Accordion.view removeObjectForm.showTriangleData
