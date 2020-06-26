module Page.SeamCarving.ContentAmplificationFormView exposing (viewContentAmplificationForm)

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


viewContentAmplificationForm : Model -> Html Msg
viewContentAmplificationForm model =
    let
        viewImage =
            model.selectedImage
                |> Maybe.map (\s -> div [ Spacing.mt2 ] [ img [ src s ] [] ])
                |> Maybe.withDefault EH.none

        isAmpButtonDisabled =
            not <| RD.isNotAsked model.contentAmplificationResp
    in
    Card.config []
        |> Card.header [] [ text "Content Amplification Form" ]
        |> Card.block []
            [ CardBlock.custom <|
                div []
                    [ viewHelp model.contentAmplificationForm
                    , viewContentAmplificationForm_ model
                    , viewImage
                    ]
            ]
        |> Card.footer []
            [ Button.button
                [ Button.primary
                , Button.onClick AmplifyImage
                , Button.disabled isAmpButtonDisabled
                ]
                [ text "Amp" ]
            ]
        |> Card.view


viewContentAmplificationForm_ : Model -> Html Msg
viewContentAmplificationForm_ model =
    let
        viewDropdown =
            Dropdown.dropdown model.contentAmplificationForm.factorDropdown
                { options = []
                , toggleMsg = \s -> ContentAmplificationFormMsg <| FactorDropdown s
                , toggleButton =
                    Dropdown.toggle
                        [ Button.outlinePrimary ]
                        [ text <| String.fromFloat model.contentAmplificationForm.factor ]
                , items =
                    List.map
                        (\s ->
                            Dropdown.buttonItem
                                [ onClick <| ContentAmplificationFormMsg <| SetFactor s ]
                                [ text <| String.fromFloat s ]
                        )
                        factorRange
                }
    in
    div [ Spacing.mt3 ]
        [ Form.group []
            [ Form.label [] [ text "Set Factor: " ]
            , br [] []
            , viewDropdown
            ]
        ]


viewHelp : ContentAmplificationForm -> Html Msg
viewHelp caForm =
    let
        msg =
            ContentAmplificationFormMsg << ContentAmplificationHelpAccordionMsg

        exampleBeforeSrc =
            "https://seam-carving.s3-us-west-2.amazonaws.com/examples/contentAmplification/input.png"

        exampleAfterSrc =
            "http://seam-carving.s3-us-west-2.amazonaws.com/examples/contentAmplification/output.png"
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
                            [ text "Upscales the image by "
                            , b [] [ text "Factor" ]
                            , text " and then seam carves the image down to the original size."
                            , br [] []
                            , text "Makes the high energy content more prominant in the photo. In the below example, the content was amplified by a factor of 1.6"
                            , div [ Flex.block, Spacing.mt3 ]
                                [ img [ src exampleBeforeSrc ] []
                                , img [ src "assets/images/next.svg", Spacing.mx2 ] []
                                , img [ src exampleAfterSrc ] []
                                ]
                            ]
                        ]
                    ]
                }
            ]
        |> Accordion.view caForm.showHelp
