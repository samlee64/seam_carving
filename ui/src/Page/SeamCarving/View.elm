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
import Data.Markings exposing (..)
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
import Page.SeamCarving.Model exposing (..)
import Page.SeamCarving.Msg exposing (..)
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

        viewImage title =
            Card.config [ Card.attrs [ onClick <| SelectImage title ] ]
                |> Card.header [] [ text title ]
                |> Card.block []
                    [ CardBlock.custom <|
                        img [ src (imgSrc title) ] []
                    ]
                |> Card.footer [] []
                |> Card.view
    in
    imageTitles
        |> List.map viewImage
        |> div [ Flex.block, Flex.wrap ]


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
                        [ Checkbox.checked True, Checkbox.onCheck (GrowFormMsg << ShowGrowIntermediateSteps) ]
                        "Show Intermediate Steps"
                    , Form.group []
                        [ Form.label [] [ text "Add Width (pixels)" ]
                        , Input.number
                            [ Input.attrs [ placeholder "width pixels" ]
                            , Input.onInput (GrowFormMsg << SetWidth)
                            , Input.value widthValue
                            ]
                        ]
                    , Form.group []
                        [ Form.label [] [ text "Add Height (pixels)" ]
                        , Input.number
                            [ Input.attrs [ placeholder "height pixels" ]
                            , Input.onInput (GrowFormMsg << SetHeight)
                            , Input.value heightValue
                            ]
                        ]
                    , Form.group []
                        [ Form.label [] [ text "Set Number of Steps" ]
                        , div [] [ text "Specifies how many times the energy map should be cleared. Also specifies max number of times any given pixel can be copied" ]
                        , Dropdown.dropdown model.growForm.numStepsDropdown
                            { options = []
                            , toggleMsg = GrowFormMsg << NumStepsDropdown
                            , toggleButton =
                                Dropdown.toggle
                                    [ Button.outlinePrimary ]
                                    [ text <| String.fromInt model.growForm.numSteps ]
                            , items =
                                List.map
                                    (\s ->
                                        Dropdown.buttonItem
                                            [ onClick <| GrowFormMsg <| SetNumSteps s ]
                                            [ text <| String.fromInt s ]
                                    )
                                    numSteps
                            }
                        ]
                    ]
            ]
        |> Card.footer []
            [ Button.button
                [ Button.primary
                , Button.onClick GrowImage
                , Button.disabled isGrowButtonDisabled
                ]
                [ text "Grow" ]
            , div [ Spacing.mt2, Flex.block ]
                [ viewWebData (\resp -> viewExecutionStatus resp.status) model.pollExecutionStatusResp ]
            ]
        |> Card.view


viewContentAmplification : Model -> Html Msg
viewContentAmplification model =
    div []
        [ viewContentAmplificationForm model
        , viewResults model
        ]


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
                    ]
            ]
        |> Card.footer []
            [ Button.button
                [ Button.primary
                , Button.onClick AmplifyImage
                ]
                [ text "Amp" ]
            , div
                [ Spacing.mt2, Flex.block ]
                [ viewWebData (\resp -> viewExecutionStatus resp.status) model.pollExecutionStatusResp ]
            ]
        |> Card.view


viewObjectRemoval : Model -> Html Msg
viewObjectRemoval model =
    div []
        [ viewObjectRemovalForm model
        , viewResults model
        ]


viewObjectRemovalForm : Model -> Html Msg
viewObjectRemovalForm model =
    Card.config []
        |> Card.header [] [ text "Object Removal Form" ]
        |> Card.block []
            [ CardBlock.custom <| viewCanvasControls model
            , CardBlock.custom <| viewCanvas model
            ]
        |> Card.footer []
            [ div [ Spacing.mt2, Flex.block ]
                [ viewWebData (\resp -> viewExecutionStatus resp.status) model.pollExecutionStatusResp ]
            ]
        |> Card.view


viewCanvasControls : Model -> Html Msg
viewCanvasControls ({ removeObjectForm } as model) =
    let
        viewModeInfo form =
            let
                clickMode =
                    form.clickMode
                        == Continious
                        |> Extra.ternary
                            (Badge.badgeLight [] [ text "Continious" ])
                            (Badge.badgeDark [] [ text "Discreet" ])

                markMode =
                    form.markMode
                        == Protect
                        |> Extra.ternary
                            (Badge.badgeSuccess [] [ text "Protect" ])
                            (Badge.badgeDanger [] [ text "Destroy" ])
            in
            div []
                [ h5 [] [ text "ClickMode: ", clickMode ]
                , h5 [] [ text "MarkMode: ", markMode ]
                ]
    in
    Html.map RemoveObjectFormMsg <|
        div []
            [ viewTriangleData model
            , div [ Spacing.mt3 ]
                [ viewModeInfo removeObjectForm
                , div [ Spacing.mb2 ]
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
            ]


viewCanvas : Model -> Html Msg
viewCanvas ({ removeObjectForm } as model) =
    let
        imgSrc =
            model.selectedImage
                |> Maybe.map (\si -> model.flags.bucket ++ "/defaults/" ++ si ++ ".png")
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
            model.selectedImage |> Maybe.withDefault ""

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
        viewTri data =
            div []
                [ text <| "one: " ++ E.encode 0 (E.list E.int data.one)
                , br [] []
                , text <| "two: " ++ E.encode 0 (E.list E.int data.two)
                , br [] []
                , text <| "three: " ++ E.encode 0 (E.list E.int data.three)
                ]

        triToString data =
            E.encode 0 (E.list (E.list E.int) [ data.one, data.two, data.three ])

        viewCoords data =
            div [ Spacing.m2 ] [ text <| triToString data ]
    in
    Accordion.config AccordionMsg
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

        isOutputImage key =
            key
                |> String.split "/"
                |> LE.last
                |> Maybe.map ((==) "output.png")
                |> Maybe.withDefault False

        ( output, rest ) =
            model.pollExecutionStatusResp
                |> RD.toMaybe
                |> Maybe.andThen .s3Url
                |> Maybe.map (List.partition isOutputImage)
                |> Maybe.withDefault ( [], [] )

        viewOutputImage =
            Card.config [ Card.attrs [ Spacing.mt3 ] ]
                |> Card.header [] [ text "Output" ]
                |> Card.block []
                    [ List.map
                        (\s3Key ->
                            img [ src (imgSrc s3Key) ] []
                        )
                        output
                        |> div []
                        |> CardBlock.custom
                    ]
                |> Card.view

        viewRest =
            Card.config [ Card.attrs [ Spacing.mt3 ] ]
                |> Card.header [] [ text "Intermediate Steps" ]
                |> Card.block []
                    [ List.map
                        (\s3Key ->
                            img [ src (imgSrc s3Key) ] []
                        )
                        rest
                        |> div []
                        |> CardBlock.custom
                    ]
                |> Card.view
    in
    div [ Flex.block, Flex.wrap ] [ viewOutputImage, viewRest ]



--    div [ Flex.block, Flex.wrap ]
--        [ viewWebData
--            (\r ->
--                r.s3Url
--                    |> Maybe.map (div [] << List.map viewResult)
--                    |> Maybe.withDefault EH.none
--            )
--            model.pollExecutionStatusResp
--        ]


viewResults2 : Model -> Html Msg
viewResults2 model =
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
                    (div [] [ img [ src (imgSrc s3Key) ] [] ])

        isOutputImage key =
            key
                |> String.split "/"
                |> LE.last
                |> Maybe.map ((==) "output.png")
                |> Maybe.withDefault False

        viewMaybeOutputImage =
            testImages
                |> List.partition isOutputImage
                |> Tuple.first
                |> List.head
                |> Maybe.map viewOutputImage
                |> Maybe.withDefault EH.none

        viewOutputImage s3Key =
            Card.config []
                |> Card.header [] [ text "Output" ]
                |> Card.block [] [ CardBlock.custom <| img [ src (imgSrc s3Key) ] [] ]
                |> Card.view

        viewRest =
            testImages
                |> List.partition isOutputImage
                |> Tuple.second
                |> List.map viewResult
                |> div [ Flex.block, Flex.wrap ]
    in
    div [ Flex.block, Flex.wrap ]
        [ viewMaybeOutputImage
        , viewRest
        ]


testImages : List String
testImages =
    [ "output/removeObject/dolphin/79e85a93-660e-47ea-8bbe-adb274f7b785/combined-mask.png"
    , "output/removeObject/dolphin/79e85a93-660e-47ea-8bbe-adb274f7b785/energy-map.gif"
    , "output/removeObject/dolphin/79e85a93-660e-47ea-8bbe-adb274f7b785/mid.gif"
    , "output/removeObject/dolphin/79e85a93-660e-47ea-8bbe-adb274f7b785/output.png"
    , "output/removeObject/dolphin/79e85a93-660e-47ea-8bbe-adb274f7b785/remove.gif"
    , "output/removeObject/dolphin/79e85a93-660e-47ea-8bbe-adb274f7b785/protect-destroy-areas.png"
    ]
