import React, { FormEvent, HTMLProps, useCallback, useState } from "react";
import { useTranslation } from "react-i18next";
import api2 from "../../../generated/.api_doc_parameters.json";
import BasicInput from "../../BasicInput/BasicInput";
import BasicForm from "../BasicForm.scss";

const formStyle: React.CSSProperties = { marginTop: "1.5em" };

const FIELDS = {
  login: api2.userSignIn.post.login,
  password: api2.userSignIn.post.password
};

type Props = {
  fieldsetProperties?: HTMLProps<HTMLFieldSetElement>;
  formProperties?: HTMLProps<HTMLFormElement>;
  id: string;
  title?: string;
};

const loginInputProperties = { required: FIELDS.login.required };
const passwordInputProperties = { required: FIELDS.password.required };

/**
 * onSubmit function
 * @param event the on submit form event
 */
const onSubmitRequest = async (
  event: FormEvent<HTMLFormElement>,
  pendingUpdater: (state: boolean) => void,
  resultUpdater: (success: boolean, error?: string) => void
): Promise<void> => {
  event.preventDefault();
  const fields = [FIELDS.login.name, FIELDS.password.name];
  const formElements = event.currentTarget.elements;

  const formData = new Map<string, string>(
    fields.map((fieldName: string) => {
      const fieldInput = formElements.namedItem(fieldName);
      if (!fieldInput || !(fieldInput instanceof HTMLInputElement)) {
        throw Error("Invalid form usage");
      }
      return [fieldName, fieldInput.value];
    })
  );

  console.info("start spinner and lock button");
  pendingUpdater(true);
  try {
    const response = await fetch(
      event.currentTarget.getAttribute("action") || "/",
      {
        method: event.currentTarget.getAttribute("method") || "POST",
        headers: {
          "Content-Type": "application/json"
        },
        body: JSON.stringify(Object.fromEntries(formData))
      }
    );

    if (response.status <= 400) {
      console.info("display success notification");
      resultUpdater(true);
    } else {
      console.error("display error notification with code", response.status);
      resultUpdater(false, `Error notification with code ${response.status}`);
    }
  } catch (error) {
    console.error("display error notification", error);
    const message = error instanceof Error ? error.message : "unknown error";
    resultUpdater(false, `Error notification ${message}`);
  }
  console.info("end spinner and enable button");
  pendingUpdater(false);
};

function SignIn({ fieldsetProperties, formProperties, id, title }: Props) {
  const { t } = useTranslation();
  const [pendingState, setPendingState] = useState(false);
  const [errors, setErrors] = useState<Array<string>>([]);
  const onSubmit = useCallback(
    (event) => {
      onSubmitRequest(
        event,
        setPendingState,
        (success: boolean, error?: string) => {
          if (success) {
            console.log("success");
          }
          if (error) {
            setErrors([...errors, error]);
          }
        }
      );
    },
    [errors]
  );

  return (
    <>
      <form
        style={formStyle}
        method="POST"
        action={`/api${api2.userSignIn.path}`}
        id={id}
        onSubmit={onSubmit}
        className={BasicForm.BasicForm}
        {...formProperties}
      >
        <fieldset {...fieldsetProperties}>
          <legend>{t(title || "FORMS.SIGN_IN.TITLE")}</legend>
          <BasicInput
            type="text"
            name={FIELDS.login.name}
            id="FORMS.SIGN_IN.FIELDS.IDENTIFIER"
            inputProperties={loginInputProperties}
            label={t("FORMS.SIGN_IN.FIELDS.IDENTIFIER.LABEL")}
          />

          <BasicInput
            type="password"
            name={FIELDS.password.name}
            id="FORMS.SIGN_IN.FIELDS.PASSWORD"
            inputProperties={passwordInputProperties}
            label={t("FORMS.SIGN_IN.FIELDS.PASSWORD.LABEL")}
          />
          <input
            disabled={pendingState}
            type="submit"
            value={String(t("FORMS.SIGN_IN.FIELDS.SUBMIT.LABEL"))}
          />
        </fieldset>
      </form>
      {pendingState ? <div>loading</div> : null}
    </>
  );
}

export default SignIn;
