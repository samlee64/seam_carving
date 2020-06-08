module Page.SeamCarving.View exposing (view)

import Bootstrap.Alert as Alert
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
import Canvas
import Canvas.Settings
import Canvas.Settings.Advanced
import Canvas.Settings.Line
import Data.Mouse exposing (..)
import Data.SeamCarving exposing (Status(..), statusToString)
import Data.Triangle as Triangle exposing (Triangle)
import Extra.Extra as Extra
import Extra.Html as EH
import Flags exposing (Flags)
import Html exposing (..)
import Html.Attributes exposing (..)
import Html.Events exposing (on, onClick)
import Json.Decode as Decode
import Json.Encode as E
import Page.SeamCarving.Model exposing (..)
import Page.SeamCarving.Msg exposing (..)
import RemoteData as RD exposing (RemoteData(..), WebData)
import Tuple
import View.WebData exposing (viewWebData, viewWebDataButton)


view : Model -> Html Msg
view model =
    let
        imgSrc =
            "http://seam-carving.s3-us-west-2.amazonaws.com/defaults/dolphin.png"
    in
    div []
        [ viewToolbar model
        , model.selectedImage
            |> Maybe.map (viewSelectedImage model)
            |> Maybe.withDefault (viewAllImages model)
        ]


viewToolbar : Model -> Html Msg
viewToolbar model =
    div []
        [ model.selectedImage
            |> Maybe.map (\_ -> Button.button [ Button.primary, Button.onClick UnselectImage ] [ text "Back" ])
            |> Maybe.withDefault EH.none
        ]


viewAllImages : Model -> Html Msg
viewAllImages model =
    let
        imgSrc title =
            model.flags.bucket ++ "/defaults/" ++ title ++ ".png"
    in
    imageTitles
        |> List.map (viewImage model.flags)
        |> div [ Flex.block, Flex.wrap ]


viewSelectedImage : Model -> String -> Html Msg
viewSelectedImage model imageTitle =
    Tab.config TabMsg
        |> Tab.withAnimation
        |> Tab.items
            [ Tab.item
                { id = "object-removal"
                , link = Tab.link [] [ text "Object Removal" ]
                , pane = Tab.pane [ Spacing.mt3 ] [ viewObjectRemovalForm model ]
                }
            , Tab.item
                { id = "content-amplification"
                , link = Tab.link [] [ text "Content Amplification" ]
                , pane = Tab.pane [ Spacing.mt3 ] [ viewContentAmplificationForm model ]
                }
            , Tab.item
                { id = "grow"
                , link = Tab.link [] [ text "Grow" ]
                , pane = Tab.pane [ Spacing.mt3 ] [ viewGrowForm model ]
                }
            ]
        |> Tab.view model.tabState


viewSelectedImage_ : Model -> String -> Html Msg
viewSelectedImage_ model imageTitle =
    let
        imgSrc =
            model.flags.bucket ++ "/defaults/" ++ imageTitle ++ ".png"

        gifSrc =
            model.flags.bucket ++ "/output/" ++ imageTitle ++ "/"

        viewGifs =
            gifTypes
                |> List.map (\g -> img [ src (gifSrc ++ g ++ ".gif") ] [])
                |> div []
    in
    Card.config [ Card.attrs [] ]
        |> Card.header [] [ text imageTitle ]
        |> Card.block []
            [ CardBlock.custom <|
                div []
                    [ img [ src imgSrc ]
                        []
                    , model
                        |> isExecutionDone
                        |> Extra.ternary viewGifs EH.none
                    ]
            ]
        |> Card.footer [] []
        |> Card.view


viewHealthCheck : Model -> Html Msg
viewHealthCheck model =
    div []
        [ Button.button [ Button.primary, Button.onClick HealthCheck ] [ text "Check health" ]
        , model.healthCheck |> viewWebData (\_ -> text "You are healthy")
        ]


viewImage : Flags -> String -> Html Msg
viewImage flags title =
    let
        imgSrc =
            flags.bucket ++ "/defaults/" ++ title ++ ".png"
    in
    Card.config [ Card.attrs [ onClick <| SelectImage title ] ]
        |> Card.header [] [ text title ]
        |> Card.block []
            [ CardBlock.custom <|
                img [ src imgSrc ] []
            ]
        |> Card.footer [] []
        |> Card.view


viewGrowForm : Model -> Html Msg
viewGrowForm model =
    let
        heightValue =
            model.growForm.addHeight
                |> Maybe.map String.fromInt
                |> Maybe.withDefault ""

        widthValue =
            model.growForm.addWidth
                |> Maybe.map String.fromInt
                |> Maybe.withDefault ""

        isGrowButtonDisabled =
            not <| RD.isNotAsked model.growImageResp
    in
    Card.config []
        |> Card.header [] [ text "Grow Form" ]
        |> Card.block []
            [ CardBlock.custom <|
                div []
                    [ Checkbox.checkbox
                        [ Checkbox.checked True, Checkbox.onCheck (\b -> GrowFormMsg (ShowGrowIntermediateSteps b)) ]
                        "Show Intermediate Steps"
                    , Form.group []
                        [ Form.label [] [ text "Add Width (pixels)" ]
                        , Input.number
                            [ Input.attrs [ placeholder "width pixels" ]
                            , Input.onInput (\s -> GrowFormMsg <| SetWidth s)
                            , Input.value widthValue
                            ]
                        ]
                    , Form.group []
                        [ Form.label [] [ text "Add Height (pixels)" ]
                        , Input.number
                            [ Input.attrs [ placeholder "height pixels" ]
                            , Input.onInput (\s -> GrowFormMsg <| SetHeight s)
                            , Input.value heightValue
                            ]
                        ]
                    , Form.group []
                        [ Form.label [] [ text "Set Number of Steps" ]
                        , div [] [ text "Specifies how many times the energy map should be cleared. Also specifies max number of times any given pixel can be copied" ]
                        , Dropdown.dropdown model.growForm.numStepsDropdown
                            { options = []
                            , toggleMsg = \s -> GrowFormMsg <| NumStepsDropdown s
                            , toggleButton = Dropdown.toggle [ Button.outlinePrimary ] [ text <| String.fromInt model.growForm.numSteps ]
                            , items =
                                List.map
                                    (\s ->
                                        Dropdown.buttonItem [ onClick <| GrowFormMsg <| SetNumSteps s ]
                                            [ text <| String.fromInt s ]
                                    )
                                    numSteps
                            }
                        ]
                    ]
            ]
        |> Card.footer []
            [ Button.button
                [ Button.primary, Button.onClick GrowImage, Button.disabled isGrowButtonDisabled ]
                [ text "Grow" ]
            , div [ Spacing.mt2, Flex.block ]
                [ viewWebData (\resp -> viewExecutionStatus resp.status) model.pollExecutionStatusResp ]
            ]
        |> Card.view


viewContentAmplificationForm : Model -> Html Msg
viewContentAmplificationForm model =
    Card.config []
        |> Card.header [] [ text "Content Amplification Form" ]
        |> Card.block []
            [ CardBlock.custom <|
                div []
                    [ Checkbox.checkbox
                        [ Checkbox.checked True, Checkbox.onCheck (\b -> GrowFormMsg (ShowGrowIntermediateSteps b)) ]
                        "Show Intermediate Steps"
                    , Form.group []
                        [ Form.label [] [ text "Set Factor: " ]
                        , br [] []
                        , Dropdown.dropdown model.contentAmplificationForm.factorDropdown
                            { options = []
                            , toggleMsg = \s -> ContentAmplificationFormMsg <| FactorDropdown s
                            , toggleButton = Dropdown.toggle [ Button.outlinePrimary ] [ text <| String.fromFloat model.contentAmplificationForm.factor ]
                            , items = List.map (\s -> Dropdown.buttonItem [ onClick <| ContentAmplificationFormMsg <| SetFactor s ] [ text <| String.fromFloat s ]) factorRange
                            }
                        ]
                    , viewWebData (\resp -> text "turned out ok") model.contentAmplificationResp
                    ]
            ]
        |> Card.footer []
            [ Button.button [ Button.primary, Button.onClick AmplifyImage ] [ text "Amp" ]
            ]
        |> Card.view


viewObjectRemovalForm : Model -> Html Msg
viewObjectRemovalForm model =
    Card.config []
        |> Card.header [] [ text "Object Removal Form" ]
        |> Card.block [] [ CardBlock.custom <| Html.map RemoveObjectFormMsg <| viewCanvas model ]
        |> Card.footer [] [ Button.button [ Button.primary, Button.onClick NoOp ] [ text "Remove!" ] ]
        |> Card.view


viewCanvas : Model -> Html RemoveObjectFormMsg
viewCanvas ({ removeObjectForm } as model) =
    let
        imgSrc =
            model.selectedImage
                |> Maybe.map
                    (\si ->
                        model.flags.bucket ++ "/defaults/" ++ si ++ ".png"
                    )
                |> Maybe.withDefault ""

        viewMouseData data =
            div []
                [ text <| "OffsetX: " ++ String.fromInt data.offsetX
                , br [] []
                , text <| "OffsetY: " ++ String.fromInt data.offsetY
                , br [] []
                , text <| "OffsetHeight: " ++ String.fromFloat data.offsetHeight
                , br [] []
                , text <| "OffsetWidth: " ++ String.fromFloat data.offsetWidth
                ]

        getTuple tupe =
            Maybe.map (\t -> String.fromInt (Tuple.first t) ++ ", " ++ String.fromInt (Tuple.second t)) tupe |> Maybe.withDefault ""

        viewTriangleData data =
            div []
                [ text <| "one: " ++ E.encode 0 (E.list E.int data.one)
                , br [] []
                , text <| "two: " ++ E.encode 0 (E.list E.int data.two)
                , br [] []
                , text <| "three: " ++ E.encode 0 (E.list E.int data.three)
                ]

        protected =
            removeObjectForm.mouseMoveData
                |> Maybe.map extractTriangleCoordFromMouseData
                |> Maybe.withDefault []
                |> Triangle.addCoord removeObjectForm.currTriangle
                |> Result.map (\tri -> E.encode 0 (E.list Triangle.encode (tri :: removeObjectForm.protected)))
                |> Result.withDefault ""

        destroy =
            removeObjectForm.mouseMoveData
                |> Maybe.map extractTriangleCoordFromMouseData
                |> Maybe.withDefault []
                |> Triangle.addCoord removeObjectForm.currTriangle
                |> Result.map (\tri -> E.encode 0 (E.list Triangle.encode (tri :: removeObjectForm.destroy)))
                |> Result.withDefault ""

        attributes =
            [ on "mousemove" (Decode.map MouseMove mouseMoveDataDecoder)
            , onClick Click
            , attribute "destroy" destroy
            , attribute "imgSrc" imgSrc
            , attribute "protected" protected
            ]
    in
    div []
        [ removeObjectForm.mouseMoveData
            |> Maybe.map viewMouseData
            |> Maybe.withDefault EH.none
        , div [ Flex.block, Flex.row, style "background" "grey" ] (List.map viewTriangleData removeObjectForm.protected)
        , div [ Flex.block, Flex.row, style "background" "red" ] (List.map viewTriangleData removeObjectForm.destroy)
        , div [] [ viewTriangleData removeObjectForm.currTriangle ]
        , node "remove-object" attributes []
        , div []
            [ Button.button
                [ Button.success
                , Button.onClick (SetClickMode Continious)
                , Button.disabled <| removeObjectForm.clickMode == Continious
                ]
                [ text "Set Continious" ]
            , Button.button
                [ Button.danger
                , Button.onClick (SetClickMode Discreet)
                , Button.disabled <| removeObjectForm.clickMode == Discreet
                ]
                [ text "Set Discreet" ]
            ]
        , div []
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
        ]


viewExecutionStatus : Status -> Html Msg
viewExecutionStatus status =
    let
        message =
            text <| statusToString status
    in
    case status of
        Executing ->
            Alert.simpleInfo [] [ message ]

        Uploading ->
            Alert.simpleInfo [] [ message ]

        Done ->
            Alert.simpleSuccess [] [ message ]

        Error ->
            Alert.simpleDanger [] [ message ]
