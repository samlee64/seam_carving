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
import Bootstrap.Form.Radio as Radio
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
            [ CardBlock.custom <|
                div []
                    [ viewHelp model.removeObjectForm
                    , viewCanvasControls model
                    , viewCanvas model
                    ]
            ]
        |> Card.view


viewCanvasControls : Model -> Html Msg
viewCanvasControls ({ removeObjectForm } as model) =
    let
        clickMode =
            removeObjectForm.clickMode
                == Continious
                |> Extra.ternary
                    (Badge.badgeLight [ style "cursor" "pointer" ] [ text "Continious" ])
                    (Badge.pillDark [ style "cursor" "pointer" ] [ text "Discreet" ])

        markMode =
            removeObjectForm.markMode
                == Protect
                |> Extra.ternary
                    (Badge.badgeSuccess [] [ text "Protect" ])
                    (Badge.badgeDanger [] [ text "Destroy" ])
    in
    Html.map RemoveObjectFormMsg <|
        div []
            [ div [ Spacing.my2 ]
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
                [ div [ Flex.block ]
                    [ h5 [] [ text "ClickMode: " ]
                    , Button.radioButton (removeObjectForm.clickMode == Continious)
                        [ Button.outlinePrimary
                        , Button.onClick (SetClickMode Continious)
                        ]
                        [ text "Continious" ]
                    , Button.radioButton (removeObjectForm.clickMode == Discreet)
                        [ Button.outlineSecondary
                        , Button.onClick (SetClickMode Discreet)
                        ]
                        [ text "Discreet" ]
                    ]
                , div [ Flex.block ]
                    [ h5 [] [ text "MarkMode: " ]
                    , Button.radioButton (removeObjectForm.markMode == Destroy)
                        [ Button.outlineDanger
                        , Button.onClick (SetMarkMode Destroy)
                        ]
                        [ text "Destroy" ]
                    , Button.radioButton (removeObjectForm.markMode == Protect)
                        [ Button.outlineSuccess
                        , Button.onClick (SetMarkMode Protect)
                        ]
                        [ text "Protect" ]
                    ]
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


viewHelp : RemoveObjectForm -> Html Msg
viewHelp removeObjectForm =
    let
        msg =
            RemoveObjectFormMsg << RemoveObjectHelpAccordionMsg

        exampleBeforeSrc =
            "https://seam-carving.s3-us-west-2.amazonaws.com/examples/removeObject/input.png"

        exampleAfterSrc =
            "http://seam-carving.s3-us-west-2.amazonaws.com/examples/removeObject/output.png"

        exampleBadAfterSrc =
            "http://seam-carving.s3-us-west-2.amazonaws.com/examples/removeObject/bad-output.png"
    in
    Accordion.config msg
        |> Accordion.withAnimation
        |> Accordion.cards
            [ Accordion.card
                { id = "card"
                , options = []
                , header = Accordion.header [] <| Accordion.toggle [] [ text "Help" ]
                , blocks =
                    [ Accordion.block []
                        [ CardBlock.text []
                            [ div [] [ text "Remove objects from a picture." ]
                            , div [ Spacing.mt2 ]
                                [ b [] [ text "(Required)" ]
                                , text "Mark areas to be removed with the "
                                , b [] [ text "Destroy" ]
                                , text " tool"
                                , br [] []
                                , b [] [ text "(Optional)" ]
                                , text "Mark areas in the photo that should not be touched with the "
                                , b [] [ text "Protected" ]
                                , text " tool"
                                ]
                            , div [ Spacing.mt2 ]
                                [ text "ClickMode: Continious will link triangles togeter. To exit ClickMode: Continious, enter ClickMode: Discreet and click once to finish your last triangle. "
                                , text "ClickMode: Discreet will make each triangle discreet."
                                ]
                            , div [ Spacing.mt2 ]
                                [ text "Lock Width/Height ratio will ensure that the aspect ratio of the output image is the same as in the input image."
                                ]
                            , div [ Spacing.mt2 ]
                                [ text "It is better to overmark the `Destroy' than undermark. Undermarking can lead to artifacts." ]
                            , div [ Spacing.mt4 ]
                                [ div [ Flex.block, Spacing.mt3 ]
                                    [ figure [ Spacing.mr1 ] [ img [ src exampleBeforeSrc ] [], figcaption [] [ text "Took too many photos with my ex in Japan." ] ]
                                    , figure [ Spacing.mr1 ] [ img [ src exampleAfterSrc ] [], figcaption [] [ text "Output" ] ]
                                    , figure [] [ img [ src exampleBadAfterSrc ] [], figcaption [] [ text "Undermarked 'Destroy'" ] ]
                                    ]
                                ]
                            ]
                        ]
                    ]
                }
            ]
        |> Accordion.view removeObjectForm.showHelp
