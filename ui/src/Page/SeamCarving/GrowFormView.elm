module Page.SeamCarving.GrowFormView exposing (viewGrowForm)

import Bootstrap.Accordion as Accordion
import Bootstrap.Button as Button
import Bootstrap.Card as Card
import Bootstrap.Card.Block as CardBlock
import Bootstrap.Dropdown as Dropdown
import Bootstrap.Form as Form
import Bootstrap.Form.Checkbox as Checkbox
import Bootstrap.Form.Input as Input
import Bootstrap.Utilities.Flex as Flex
import Bootstrap.Utilities.Spacing as Spacing
import Extra.Html as EH
import Html exposing (..)
import Html.Attributes exposing (..)
import Html.Events exposing (on, onClick)
import Page.SeamCarving.Model exposing (..)
import Page.SeamCarving.Msg exposing (..)
import RemoteData as RD exposing (RemoteData(..), WebData)


viewGrowForm : Model -> Html Msg
viewGrowForm ({ growForm } as model) =
    let
        isGrowButtonDisabled =
            not <| RD.isNotAsked model.growImageResp

        viewImage =
            model.selectedImage
                |> Maybe.map (\s -> div [ Spacing.mt2 ] [ img [ src s ] [] ])
                |> Maybe.withDefault EH.none
    in
    Card.config []
        |> Card.header [] [ text "Grow Form" ]
        |> Card.block []
            [ CardBlock.custom <|
                div []
                    [ viewHelp model.growForm
                    , viewGrowForm_ model
                    , viewImage
                    ]
            ]
        |> Card.footer []
            [ Button.button
                [ Button.primary
                , Button.onClick GrowImage
                , Button.disabled isGrowButtonDisabled
                ]
                [ text "Grow" ]
            ]
        |> Card.view


viewGrowForm_ : Model -> Html Msg
viewGrowForm_ ({ growForm } as model) =
    let
        heightValue =
            growForm.addHeight
                |> Maybe.map String.fromInt
                |> Maybe.withDefault ""

        widthValue =
            growForm.addWidth
                |> Maybe.map String.fromInt
                |> Maybe.withDefault ""
    in
    div [ Spacing.mt3 ]
        [ Form.group []
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
            [ div [] [ Form.label [] [ text "Set Number of Steps. Specifies how many times the energy map should be cleared. Also specifies max number of times any given pixel can be copied" ] ]
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


viewHelp : GrowForm -> Html Msg
viewHelp growForm =
    let
        msg =
            GrowFormMsg << GrowHelpAccordionMsg

        exampleBeforeSrc =
            "https://seam-carving.s3-us-west-2.amazonaws.com/examples/grow/input.png"

        example1StepSrc =
            "http://seam-carving.s3-us-west-2.amazonaws.com/examples/grow/output1.png"

        example3StepSrc =
            "http://seam-carving.s3-us-west-2.amazonaws.com/examples/grow/output3.png"

        example8StepSrc =
            "http://seam-carving.s3-us-west-2.amazonaws.com/examples/grow/output8.png"
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
                            [ div []
                                [ text "Increases the size of the image by finding low energy seams and inserting interpolated pixel values next to those seams."
                                , br [] []
                                , text "Once pixels have been inserted, that seam will not be used again to increase the image size for that step."
                                ]
                            , div [ Spacing.mt2 ]
                                [ text "Increasing the number of steps causes the same low energy seams to be used multiple times and can lead to blurred areas where the same pixel values are being interpolated over and over."
                                , br [] []
                                , text "Decreasing the number of steps to low will cause higher and higher energy seams to be duplicated, causing stretching and morphing of meaningful content."
                                ]
                            , div [ Spacing.mt2 ]
                                [ text "Here is a 320x217px photo with 150px width added."
                                , div [ Flex.block, Spacing.mt3 ]
                                    [ figure [ Spacing.mr1 ] [ img [ src exampleBeforeSrc ] [], figcaption [] [ text "Input" ] ]
                                    , figure [ Spacing.mr1 ] [ img [ src example1StepSrc ] [], figcaption [] [ text "1 Step" ] ]
                                    , figure [ Spacing.mr1 ] [ img [ src example3StepSrc ] [], figcaption [] [ text "3 Step" ] ]
                                    , figure [] [ img [ src example8StepSrc ] [], figcaption [] [ text "8 Step" ] ]
                                    ]
                                ]
                            ]
                        ]
                    ]
                }
            ]
        |> Accordion.view growForm.showHelp
