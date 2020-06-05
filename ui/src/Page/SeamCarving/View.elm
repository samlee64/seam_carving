module Page.SeamCarving.View exposing (view)

import Bootstrap.Alert as Alert
import Bootstrap.Button as Button
import Bootstrap.Card as Card
import Bootstrap.Card.Block as CardBlock
import Bootstrap.Dropdown as Dropdown
import Bootstrap.Form as Form
import Bootstrap.Form.Checkbox as Checkbox
import Bootstrap.Form.Input as Input
import Bootstrap.Utilities.Flex as Flex
import Bootstrap.Utilities.Spacing as Spacing
import Data.SeamCarving exposing (Status(..), statusToString)
import Extra.Extra as Extra
import Extra.Html as EH
import Flags exposing (Flags)
import Html exposing (..)
import Html.Attributes exposing (..)
import Html.Events exposing (onClick)
import Page.SeamCarving.Model exposing (Model, factorRange, gifTypes, imageTitles, isExecutionDone, numSteps)
import Page.SeamCarving.Msg exposing (..)
import RemoteData as RD exposing (RemoteData(..), WebData)
import View.WebData exposing (viewWebData, viewWebDataButton)



--TODO add some scaffolding around this switch


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
    in
    imageTitles
        |> List.map (viewImage model.flags)
        |> div [ Flex.block, Flex.wrap ]


viewSelectedImage : Model -> String -> Html Msg
viewSelectedImage model imageTitle =
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
    div []
        [ viewGrowForm model
        , viewContentAmplificationForm model
        , Card.config [ Card.attrs [] ]
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
        ]


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
                            , toggleButton = Dropdown.toggle [ Button.outlinePrimary ] [ text <| String.fromInt model.contentAmplificationForm.factor ]
                            , items = List.map (\s -> Dropdown.buttonItem [ onClick <| ContentAmplificationFormMsg <| SetFactor s ] [ text <| String.fromInt s ]) factorRange
                            }
                        ]
                    , viewWebData (\resp -> text "turned out ok") model.contentAmplificationResp
                    ]
            ]
        |> Card.footer []
            [ Button.button [ Button.primary, Button.onClick AmplifyImage ] [ text "Amp" ]
            ]
        |> Card.view


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
